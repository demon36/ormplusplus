#include <logger.h>
#include <typeindex>
#include <vector>

#include <mysql/mysql.h>
#include <mysql_session.h>


using namespace std;

namespace ormplusplus {

void mysql_session::mysql_query(const std::string& query){
	if (mysql_query((st_mysql*)session_ptr, query.c_str())) {
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}
}

const type_info& mysql_session::get_type_info(int mySQLTypeEnum){
	//todo: revise typing setup
	enum_field_types mySQLType = (enum_field_types)mySQLTypeEnum;
	switch(mySQLType){
//		case MYSQL_TYPE_BIT://todo: support bool
		case MYSQL_TYPE_TINY:
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			return Integer::get_type_info();

		case MYSQL_TYPE_LONGLONG:
			return Long::get_type_info();


		case MYSQL_TYPE_FLOAT:
			return Float::get_type_info();
		case MYSQL_TYPE_DOUBLE:
		case MYSQL_TYPE_DECIMAL:
			return Double::get_type_info();

		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_NEWDATE:
			return DateTime::get_type_info();

		case MYSQL_TYPE_VARCHAR:
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_BLOB://todo: support blob
			return String::get_type_info();

		case MYSQL_TYPE_NULL:
			return Null::get_type_info();

		default:
			throw runtime_error("unsupported type");
	}
}

const map<string, type_info> mysql_session::type_names_map({
	{"INT", Integer::get_type_info()},
	{"BIGINT", Long::get_type_info()},
	{"FLOAT", Float::get_type_info()},
	{"DOUBLE", Double::get_type_info()},
	{"VARCHAR", String::get_type_info()},
	{"DATETIME", DateTime::get_type_info()},
});

const type_info& mysql_session::get_type_info(const std::string& mySQLColTypeName){
	string normalizedCaseName = mySQLColTypeName;
	for(char &c: normalizedCaseName){
		c = (char)toupper(c);
	}

	if(type_names_map.find(normalizedCaseName) == type_names_map.end()){
		throw out_of_range("MySQLSession::getTypeInfo called with unsupported type name");
	}else{
		return type_names_map.find(normalizedCaseName)->second;
	}
}

mysql_session::mysql_session(const string& host, const string& database, const string& user, const string& password, int port){
	session_ptr = mysql_init(NULL);

	if(session_ptr == NULL){
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}

	//todo: use compression
	if (mysql_real_connect((st_mysql*)session_ptr, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0) == NULL){
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}

	ORMLOG(logger::lvl::INFO, "connected to mysql server " + host + ":" + to_string(port));
}

bool mysql_session::table_exists(const string& name){
	string query = "SHOW TABLES LIKE '"+name+"';";
	result_table foundTables = execute_flat(query);
	return !foundTables.get_num_rows() == 0;
}

void mysql_session::create_table(const string& name, const table_schema& schema){
	stringstream queryStream;
	queryStream << "CREATE TABLE IF NOT EXISTS `"<< name <<"`(";
	std::vector<table_column> columnsList;
	for(auto& columnEntry : schema ) {
		columnsList.push_back( columnEntry.second );
	}


	for(size_t i = 0; i < columnsList.size(); i++){
		string DBTypeName;
		for(auto& typeNameEntry : type_names_map){
			if(typeNameEntry.second.wrapper_type_hash == columnsList[i].get_type_info().wrapper_type_hash){
				DBTypeName = typeNameEntry.first;
				break;
			}
		}

		if(DBTypeName.empty()){
			throw runtime_error("trying to create table with column '"+columnsList[i].get_name()+"' having unsupported type");
		}

		queryStream << "`"<< columnsList[i].get_name() <<"` " <<  DBTypeName;
		if(columnsList[i].is_text()){
			queryStream << "(" << columnsList[i].get_length() << ")";
		}
		if(columnsList[i].is_auto_increment()){
			queryStream << " AUTO_INCREMENT ";
		}

		if(columnsList[i].has_default_value()){
			if(columnsList[i].get_default_value().is_null()){
				queryStream << " DEFAULT NULL ";
			}else{
				queryStream << " DEFAULT '" << columnsList[i].get_default_value() << "' ";
			}
		}

		if(columnsList[i].is_primary_key()){
			queryStream << " PRIMARY KEY ";
		}
		if(columnsList[i].is_nullable()){
			queryStream << " NULL ";
		}else{
			queryStream << " NOT NULL ";
		}
		if(i < columnsList.size()-1){
			queryStream << ", ";
		}
	}

	queryStream << ");";

	execute_void(queryStream.str());
}

table_schema mysql_session::get_table_schema(const string& name){
	//TODO: use an ORM class for schema
	string query = "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, NUMERIC_PRECISION, IS_NULLABLE, COLUMN_DEFAULT, COLUMN_KEY, EXTRA "
			"FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+name+"';";
	result_table result = execute_flat(query);
	table_schema schema;
	for(size_t i = 0; i < result.get_num_rows(); i++){
		string columnName = result.get_field_value(i, "COLUMN_NAME").getValueRef<string>();
		string DBColumnType = result.get_field_value(i, "DATA_TYPE").getValueRef<string>();
		string extra = result.get_field_value(i, "EXTRA").getValueRef<string>();
		long maxLength = result.get_field_value(i, "CHARACTER_MAXIMUM_LENGTH").is_null() ? -1 : result.get_field_value(i, "CHARACTER_MAXIMUM_LENGTH").getValueRef<long>();
		long numPrecision = result.get_field_value(i, "NUMERIC_PRECISION").is_null() ? -1 : result.get_field_value(i, "NUMERIC_PRECISION").getValueRef<long>();
		bool isNullable = result.get_field_value(i, "IS_NULLABLE").getValueRef<string>() == "YES";
		bool isPKey = result.get_field_value(i, "COLUMN_KEY").getValueRef<string>() == "PRI";
		bool isAutoIncrement = extra.find("auto_increment") != extra.npos;

		table_column tempColumn(
				columnName,
				get_type_info(DBColumnType),
				maxLength,
				numPrecision,
				isNullable,
				isPKey,
				isAutoIncrement
		);

		if(!result.get_field_value(i, "COLUMN_DEFAULT").is_null()){
			String defaultValue = result.get_field_value(i, "COLUMN_DEFAULT").getValueRef<string>();
			if(defaultValue == "NULL"){
				tempColumn.set_default_value(String());
			}else if(tempColumn.get_type_info() == String::get_type_info()){
				//default value is wrapped in single quotations
				tempColumn.set_default_value(defaultValue.getValueRef().substr(1, defaultValue.getValueRef().size()-2));
			}else{
				tempColumn.set_default_value(defaultValue);
			}
		}

		schema.emplace(columnName, tempColumn);
	}
	return schema;
}

void mysql_session::insert(model_base& model, bool updateAutoIncPKey){
	stringstream queryStream;
	const table_schema schema = model.get_schema();
	queryStream << "INSERT INTO " << model.get_table_name();
	queryStream << " ( ";
	//will rely on map internal order, same with attribute values
	print_col_names(queryStream, model.get_schema());
	queryStream << " ) VALUES ( ";
	print_attrib_values(queryStream, model.get_schema(), model.get_attribs());
	queryStream << " );";
	if(execute_void(queryStream.str()) != 1){
		throw runtime_error("failed to insert model");
	}

	if(updateAutoIncPKey && model.auto_inc_pkey_col_exists()){
		long lastInsertId = mysql_insert_id((st_mysql*)session_ptr); //  result.begin()->get(0).convert<long>();
		model.set_auto_inc_pkey(lastInsertId);
	}
	return;
}

result_table mysql_session::execute_flat(const query_base& query){
	std::string queryString = build_query_string(query);
	return execute_flat(queryString);
}

result_table mysql_session::execute_flat(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	mysql_query(query);

	MYSQL_RES* result = mysql_store_result((st_mysql*)session_ptr);

	if(result == NULL){
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}

	MYSQL_ROW row;
	MYSQL_FIELD *field;
	vector<string> columns;
	vector<const type_info*> columnTypes;

	int num_fields = mysql_num_fields(result);

	while((field = mysql_fetch_field(result)) != NULL){
		columns.push_back(field->name);
		columnTypes.push_back(&get_type_info(field->type));
	}

	result_table flatResults(columns, columnTypes);

	while((row = mysql_fetch_row(result)) != NULL){
		ORMLOG(logger::lvl::DBUG, "fetching new row");
		size_t rowIdx = flatResults.add_row();
		for(int i = 0; i < num_fields; i++){
			flatResults.set_field_value(rowIdx, i, row[i]);
		}
	}

	mysql_free_result(result);
	return flatResults;
}

std::size_t mysql_session::execute_void(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	mysql_query(query);
	return (size_t)mysql_affected_rows((st_mysql*)session_ptr);
}

mysql_session::~mysql_session() {
	ORMLOG(logger::lvl::INFO, "disconnected from mysql server ");
	mysql_close((st_mysql*)session_ptr);
}

} /* namespace ORMPlusPlus */
