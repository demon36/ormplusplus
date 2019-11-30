#include "MySQLSession.h"

#include <typeindex>
#include <vector>

#include <mysql/mysql.h>

#include "Logger.h"

using namespace std;

namespace ORMPlusPlus {

void MySQLSession::mysqlQuery(const std::string& query){
	if (mysql_query((st_mysql*)sessionPtr, query.c_str())) {
		throw runtime_error(mysql_error((st_mysql*)sessionPtr));
	}
}

size_t MySQLSession::toPrimitiveType(int mySQLTypeEnum){
	//todo: revice typing setup
	enum_field_types mySQLType = (enum_field_types)mySQLTypeEnum;
	switch(mySQLType){
//		case MYSQL_TYPE_BIT://todo: support bool
		case MYSQL_TYPE_TINY:
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			return typeid(int).hash_code();

		case MYSQL_TYPE_LONGLONG:
			return typeid(long).hash_code();


		case MYSQL_TYPE_FLOAT:
			return typeid(float).hash_code();
		case MYSQL_TYPE_DOUBLE:
		case MYSQL_TYPE_DECIMAL:
			return typeid(double).hash_code();

		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_NEWDATE:
			return typeid(tm).hash_code();

		case MYSQL_TYPE_VARCHAR:
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_BLOB://todo: support blob
			return typeid(string).hash_code();

		case MYSQL_TYPE_NULL:
			return typeid(nullptr).hash_code();

		default:
			throw runtime_error("unsupported type");
	}
}

const map<string, TypeInfo> MySQLSession::typeNamesMap({
	{"INT", TypeInfo::Int32Type},
	{"BIGINT", TypeInfo::Int64Type},
	{"FLOAT", TypeInfo::FloatType},
	{"DOUBLE", TypeInfo::DoubleType},
	{"VARCHAR", TypeInfo::StringType},
	{"DATETIME", TypeInfo::DateTimeType},
});

const TypeInfo& MySQLSession::getTypeInfo(const std::string& mySQLColTypeName){
	string normalizedCaseName = mySQLColTypeName;
	for(char &c: normalizedCaseName){
		c = (char)toupper(c);
	}

	if(typeNamesMap.find(normalizedCaseName) == typeNamesMap.end()){
		throw out_of_range("MySQLSession::getTypeInfo called with unsupported type name");
	}else{
		return typeNamesMap.find(normalizedCaseName)->second;
	}
}

MySQLSession::MySQLSession(const string& host, const string& database, const string& user, const string& password, int port){
	sessionPtr = mysql_init(NULL);

	if(sessionPtr == NULL){
		throw runtime_error(mysql_error((st_mysql*)sessionPtr));
	}

	//todo: use compression
	if (mysql_real_connect((st_mysql*)sessionPtr, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0) == NULL){
		throw runtime_error(mysql_error((st_mysql*)sessionPtr));
	}

	ORMLOG(Logger::Lv::INFO, "connected to mysql server " + host + ":" + to_string(port));
}

bool MySQLSession::tableExists(const string& name){
	string query = "SHOW TABLES LIKE '"+name+"';";
	ResultTable foundTables = executeFlat(query);
	return !foundTables.getNumRows() == 0;
}

void MySQLSession::createTable(const string& name, const TableSchema& schema){
	stringstream queryStream;
	queryStream << "CREATE TABLE IF NOT EXISTS `"<< name <<"`(";
	std::vector<TableColumn> columnsList;
	for(auto& columnEntry : schema ) {
		columnsList.push_back( columnEntry.second );
	}


	for(size_t i = 0; i < columnsList.size(); i++){
		string DBTypeName;
		for(auto& typeNameEntry : typeNamesMap){
			if(typeNameEntry.second.nullableTypeHash == columnsList[i].getTypeInfo().nullableTypeHash){
				DBTypeName = typeNameEntry.first;
				break;
			}
		}

		if(DBTypeName.empty()){
			throw runtime_error("trying to create table with column '"+columnsList[i].getName()+"' having unsupported type");
		}

		queryStream << "`"<< columnsList[i].getName() <<"` " <<  DBTypeName;
		if(columnsList[i].isText()){
			queryStream << "(" << columnsList[i].getLength() << ")";
		}
		if(columnsList[i].isAutoIncrement()){
			queryStream << " AUTO_INCREMENT ";
		}

		if(columnsList[i].hasDefaultValue()){
			if(columnsList[i].getDefaultValue().isNull()){
				queryStream << " DEFAULT NULL ";
			}else{
				queryStream << " DEFAULT '" << columnsList[i].getDefaultValue() << "' ";
			}
		}

		if(columnsList[i].isPrimary()){
			queryStream << " PRIMARY KEY ";
		}
		if(columnsList[i].isNullable()){
			queryStream << " NULL ";
		}else{
			queryStream << " NOT NULL ";
		}
		if(i < columnsList.size()-1){
			queryStream << ", ";
		}
	}

	queryStream << ");";

	executeVoid(queryStream.str());
}

TableSchema MySQLSession::getTableSchema(const string& name){
	//TODO: use an ORM class for schema
	string query = "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, NUMERIC_PRECISION, IS_NULLABLE, COLUMN_DEFAULT, COLUMN_KEY, EXTRA "
			"FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+name+"';";
	ResultTable result = executeFlat(query);
	TableSchema schema;
	for(size_t i = 0; i < result.getNumRows(); i++){
		string columnName = result.getFieldValue(i, "COLUMN_NAME").getValueRef<string>();
		string DBColumnType = result.getFieldValue(i, "DATA_TYPE").getValueRef<string>();
		string extra = result.getFieldValue(i, "EXTRA").getValueRef<string>();
		long maxLength = result.getFieldValue(i, "CHARACTER_MAXIMUM_LENGTH").isNull() ? -1 : result.getFieldValue(i, "CHARACTER_MAXIMUM_LENGTH").getValueRef<long>();
		long numPrecision = result.getFieldValue(i, "NUMERIC_PRECISION").isNull() ? -1 : result.getFieldValue(i, "NUMERIC_PRECISION").getValueRef<long>();
		bool isNullable = result.getFieldValue(i, "IS_NULLABLE").getValueRef<string>() == "YES";
		bool isPKey = result.getFieldValue(i, "COLUMN_KEY").getValueRef<string>() == "PRI";
		bool isAutoIncrement = extra.find("auto_increment") != extra.npos;

		TableColumn tempColumn(
				columnName,
				getTypeInfo(DBColumnType),
				maxLength,
				numPrecision,
				isNullable,
				isPKey,
				isAutoIncrement
		);

		if(!result.getFieldValue(i, "COLUMN_DEFAULT").isNull()){
			String defaultValue = result.getFieldValue(i, "COLUMN_DEFAULT").getValueRef<string>();
			if(defaultValue == "NULL"){
				tempColumn.setDefaultValue(String());
			}else if(tempColumn.getTypeInfo() == TypeInfo::StringType){
				//default value is wrapped in single quotations
				tempColumn.setDefaultValue(defaultValue.getValueRef().substr(1, defaultValue.getValueRef().size()-2));
			}else{
				tempColumn.setDefaultValue(defaultValue);
			}
		}

		schema.emplace(columnName, tempColumn);
	}
	return schema;
}

void MySQLSession::insert(ModelBase& model, bool updateAutoIncPKey){
	stringstream queryStream;
	const TableSchema schema = model.getSchema();
	queryStream << "INSERT INTO " << model.getTableName();
	queryStream << " ( ";
	//will rely on map internal order, same with attribute values
	printColumnNames(queryStream, model.getSchema());
	queryStream << " ) VALUES ( ";
	printAttribValues(queryStream, model.getSchema(), model.getAttributes());
	queryStream << " );";
	if(executeVoid(queryStream.str()) != 1){
		throw runtime_error("failed to insert model");
	}

	if(updateAutoIncPKey && model.autoIncPkeyColumnExists()){
		ResultTable result = executeFlat("SELECT LAST_INSERT_ID();");
		//const char* test = result.begin()->get(0).type().name();
		//TODO: error check
		if(result.getNumRows() == 0){
			throw runtime_error("SELECT LAST_INSERT_ID(); returned no results");
		}
		long lastInsertId = result.getFieldValue(0, 0).getValueRef<long>(); //  result.begin()->get(0).convert<long>();
		model.setAutoIncPKey(lastInsertId);
	}
	return;
}

ResultTable MySQLSession::executeFlat(const QueryBase& query){
	std::string queryString = buildQueryString(query);
	return executeFlat(queryString);
}

ResultTable MySQLSession::executeFlat(const std::string& query){
	ORMLOG(Logger::Lv::DBUG, "executing query : " + query);
	mysqlQuery(query);

	MYSQL_RES* result = mysql_store_result((st_mysql*)sessionPtr);

	if(result == NULL){
		throw runtime_error(mysql_error((st_mysql*)sessionPtr));
	}

	MYSQL_ROW row;
	MYSQL_FIELD *field;
	vector<string> columns;
	vector<size_t> columnTypeHashes;

	int num_fields = mysql_num_fields(result);

	while((field = mysql_fetch_field(result)) != NULL){
		columns.push_back(field->name);
		columnTypeHashes.push_back(toPrimitiveType(field->type));
	}

	ResultTable flatResults(columns, columnTypeHashes);

	while((row = mysql_fetch_row(result)) != NULL){
		ORMLOG(Logger::Lv::DBUG, "fetching new row");
		size_t rowIdx = flatResults.addRow();
		for(int i = 0; i < num_fields; i++){
			flatResults.setFieldValue(rowIdx, i, row[i]);
		}
	}

	mysql_free_result(result);
	return flatResults;
}

std::size_t MySQLSession::executeVoid(const std::string& query){
	ORMLOG(Logger::Lv::DBUG, "executing query : " + query);
	mysqlQuery(query);
	return (size_t)mysql_affected_rows((st_mysql*)sessionPtr);
}

MySQLSession::~MySQLSession() {
	ORMLOG(Logger::Lv::INFO, "disconnected from mysql server ");
	mysql_close((st_mysql*)sessionPtr);
}

} /* namespace ORMPlusPlus */
