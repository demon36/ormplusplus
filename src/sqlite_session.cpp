#include <logger.h>
#include <sqlite_session.h>
#include <typeindex>
#include <vector>
#include <sqlite3.h>

using namespace std;

namespace ormplusplus {

void* sqlite_session::sqlite_query(const std::string& query){
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2((sqlite3*)session_ptr, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
	}
	return stmt;
}

/**
 * @see https://www.sqlite.org/c3ref/c_blob.html
 */
const type_info& sqlite_session::get_type_info(int sqliteTypeId){
	//todo: revise typing setup
	//todo: use TypeInfo here
	//todo: BROKEN LOGIC: sqlite fundamental datatypes cannot be mapped to ormplusplus data types
	switch(sqliteTypeId){
		case SQLITE_INTEGER:
			return Integer::get_type_info();
//			return typeid(long).hash_code();

		case SQLITE_FLOAT:
			return Float::get_type_info();
//			return typeid(double).hash_code();

//		case SQLITE_TEXT:
//			return typeid(tm).hash_code();

		case SQLITE_TEXT:
			return String::get_type_info();

		case SQLITE_NULL:
			return Null::get_type_info();

		default:
			throw runtime_error("unsupported type");
	}
}

const map<string, type_info> sqlite_session::type_names_map({
	{"INT", Integer::get_type_info()},
	{"INTEGER", Long::get_type_info()},
	{"FLOAT", Float::get_type_info()},
	{"DOUBLE", Double::get_type_info()},
	{"VARCHAR", String::get_type_info()},
	{"TEXT", String::get_type_info()},
	{"DATETIME", DateTime::get_type_info()},
});

const type_info& sqlite_session::get_type_info(const std::string& mySQLColTypeName){
	string normalizedCaseName = mySQLColTypeName;
	for(char &c: normalizedCaseName){
		c = (char)toupper(c);
	}

	//todo: add length to column info
	//todo: fix this bs
	if(normalizedCaseName.find("VARCHAR") == 0){
		normalizedCaseName = "VARCHAR";
	} else if(normalizedCaseName.find("TEXT") == 0){
		normalizedCaseName = "TEXT";
	}

	if(type_names_map.find(normalizedCaseName) == type_names_map.end()){
		throw out_of_range("SQLiteSession::getTypeInfo called with unsupported type name");
	}else{
		return type_names_map.find(normalizedCaseName)->second;
	}
}

sqlite_session::sqlite_session(const string& dbPath){
    if (sqlite3_open(dbPath.c_str(), (sqlite3**)&session_ptr) != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg((sqlite3*)session_ptr));
        sqlite3_close((sqlite3*)session_ptr);
        throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
    }

	ORMLOG(logger::lvl::INFO, "connected to sqlite path " + dbPath);
}

bool sqlite_session::table_exists(const string& name){
	string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='"+name+"';";
	result_table foundTables = execute_flat(query);
	return !foundTables.get_num_rows() == 0;
}

void sqlite_session::create_table(const string& name, const table_schema& schema){
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

		if(columnsList[i].is_auto_increment()){
			queryStream << " AUTOINCREMENT ";
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

table_schema sqlite_session::get_table_schema(const string& name){
	//TODO: use an ORM class for schema
	string query = "pragma table_info('"+name+"');";
	result_table result = execute_flat(query);
	table_schema schema;
	for(size_t i = 0; i < result.get_num_rows(); i++){
		string columnName = result.get_field_value(i, "name").getValueRef<string>();
		string DBColumnType = result.get_field_value(i, "type").getValueRef<string>();
		long maxLength = -1;//todo: parse number between parentheses, ex: VARCHAR(34)
		long numPrecision = -1;//todo: understand how sqlite does it
		bool isNullable = result.get_field_value(i, "notnull").getValueRef<int>() == 0;
		bool isPKey = result.get_field_value(i, "pk").getValueRef<int>() == 1;
		bool isAutoIncrement = true;//todo: fetch autoincrement info, will probably need to parse the table creation sql :S

		table_column tempColumn(
				columnName,
				get_type_info(DBColumnType),
				maxLength,
				numPrecision,
				isNullable,
				isPKey,
				isAutoIncrement
		);

		if(!result.get_field_value(i, "dflt_value").is_null()){ //todo: add a test case for these conditions
			String defaultValue = result.get_field_value(i, "dflt_value").getValueRef<string>();
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

void sqlite_session::insert(model_base& model, bool updateAutoIncPKey){
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
		long lastInsertId = sqlite3_last_insert_rowid((sqlite3*)session_ptr); //  result.begin()->get(0).convert<long>();
		model.set_auto_inc_pkey(lastInsertId);
	}
	return;
}

result_table sqlite_session::execute_flat(const query_base& query){
	std::string queryString = build_query_string(query);
	return execute_flat(queryString);
}

result_table sqlite_session::execute_flat(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	sqlite3_stmt* stmt = (sqlite3_stmt*)sqlite_query(query);

	vector<string> columns;
	vector<const type_info*> columnTypes;

	int retval = sqlite3_step(stmt);
	if (retval != SQLITE_DONE && retval != SQLITE_ROW) {
		throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
	}

	int num_fields = sqlite3_column_count(stmt);
	for(int i = 0; i < num_fields; i++){
		columns.push_back(sqlite3_column_name(stmt, i));
		const char* sqliteTypeName = sqlite3_column_decltype(stmt, i);
		if(sqliteTypeName == NULL){
			columnTypes.push_back(&get_type_info(sqlite3_column_type(stmt, i)));
		} else {
			columnTypes.push_back(&get_type_info(sqliteTypeName));
		}
	}

	result_table flatResults(columns, columnTypes);
	while(retval == SQLITE_ROW){
		ORMLOG(logger::lvl::DBUG, "fetching new row");
		size_t rowIdx = flatResults.add_row();
		for(int i = 0; i < num_fields; i++){
			flatResults.set_field_value(rowIdx, i, (const char*)sqlite3_column_text(stmt, i));
			//todo: make use of fns like sqlite3_column_int() for better performance
		}
		retval = sqlite3_step(stmt);
	}
    sqlite3_finalize(stmt);
	return flatResults;
}

std::size_t sqlite_session::execute_void(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	sqlite3_stmt* stmt = (sqlite3_stmt*)sqlite_query(query);
	int retval = sqlite3_step(stmt);
    if (retval != SQLITE_DONE && retval != SQLITE_ROW) {
    	//todo: add app specific description to exception msgs, have a help function to wrap it
    	throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
    }
	return (size_t)sqlite3_changes((sqlite3*)session_ptr);
}

sqlite_session::~sqlite_session() {
	ORMLOG(logger::lvl::INFO, "disconnected from sqlite database");
	sqlite3_close((sqlite3*)session_ptr);
}

} /* namespace ORMPlusPlus */
