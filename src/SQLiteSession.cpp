#include "SQLiteSession.h"
#include "Logger.h"

#include <typeindex>
#include <vector>
#include <sqlite3.h>

using namespace std;

namespace ORMPlusPlus {

void* SQLiteSession::sqliteQuery(const std::string& query){
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2((sqlite3*)sessionPtr, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		throw runtime_error(sqlite3_errmsg((sqlite3*)sessionPtr));
	}
	return stmt;
}

/**
 * @see https://www.sqlite.org/c3ref/c_blob.html
 */
size_t SQLiteSession::toPrimitiveType(int sqliteTypeId){
	//todo: revise typing setup
	//todo: use TypeInfo here
	//todo: BROKEN LOGIC: sqlite fundamental datatypes cannot be mapped to ormplusplus data types
	switch(sqliteTypeId){
		case SQLITE_INTEGER:
			return typeid(int).hash_code();
//			return typeid(long).hash_code();

		case SQLITE_FLOAT:
			return typeid(float).hash_code();
//			return typeid(double).hash_code();

//		case SQLITE_TEXT:
//			return typeid(tm).hash_code();

		case SQLITE_TEXT:
			return typeid(string).hash_code();

		case SQLITE_NULL:
			return typeid(nullptr).hash_code();

		default:
			throw runtime_error("unsupported type");
	}
}

const map<string, TypeInfo> SQLiteSession::typeNamesMap({
	{"INT", Integer::getTypeInfo()},
	{"BIGINT", Long::getTypeInfo()},
	{"FLOAT", Float::getTypeInfo()},
	{"DOUBLE", Double::getTypeInfo()},
	{"VARCHAR", String::getTypeInfo()},
	{"DATETIME", DateTime::getTypeInfo()},
});

const TypeInfo& SQLiteSession::getTypeInfo(const std::string& mySQLColTypeName){
	string normalizedCaseName = mySQLColTypeName;
	for(char &c: normalizedCaseName){
		c = (char)toupper(c);
	}

	if(typeNamesMap.find(normalizedCaseName) == typeNamesMap.end()){
		throw out_of_range("SQLiteSession::getTypeInfo called with unsupported type name");
	}else{
		return typeNamesMap.find(normalizedCaseName)->second;
	}
}

SQLiteSession::SQLiteSession(const string& dbPath){
    if (sqlite3_open(dbPath.c_str(), (sqlite3**)&sessionPtr) != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg((sqlite3*)sessionPtr));
        sqlite3_close((sqlite3*)sessionPtr);
        throw runtime_error(sqlite3_errmsg((sqlite3*)sessionPtr));
    }

	ORMLOG(Logger::Lv::INFO, "connected to sqlite path " + dbPath);
}

bool SQLiteSession::tableExists(const string& name){
	string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='"+name+"';";
	ResultTable foundTables = executeFlat(query);
	return !foundTables.getNumRows() == 0;
}

void SQLiteSession::createTable(const string& name, const TableSchema& schema){
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
			queryStream << " AUTOINCREMENT ";
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

TableSchema SQLiteSession::getTableSchema(const string& name){
	//TODO: use an ORM class for schema
	string query = "pragma table_info('"+name+"');";
	ResultTable result = executeFlat(query);
	TableSchema schema;
	for(size_t i = 0; i < result.getNumRows(); i++){
		string columnName = result.getFieldValue(i, "NAME").getValueRef<string>();
		string DBColumnType = result.getFieldValue(i, "TYPE").getValueRef<string>();
		long maxLength = -1;//todo: parse number between parentheses, ex: VARCHAR(34)
		long numPrecision = -1;//todo: understand how sqlite does it
		bool isNullable = result.getFieldValue(i, "NOTNULL").getValueRef<string>() == "0";
		bool isPKey = result.getFieldValue(i, "PK").getValueRef<string>() == "1";
		bool isAutoIncrement = true;//todo: fetch autoincrement info, will probably need to parse the table creation sql :S

		TableColumn tempColumn(
				columnName,
				getTypeInfo(DBColumnType),
				maxLength,
				numPrecision,
				isNullable,
				isPKey,
				isAutoIncrement
		);

		if(!result.getFieldValue(i, "DFLT_VALUE").isNull()){ //todo: add a test case for these conditions
			String defaultValue = result.getFieldValue(i, "DFLT_VALUE").getValueRef<string>();
			if(defaultValue == "NULL"){
				tempColumn.setDefaultValue(String());
			}else if(tempColumn.getTypeInfo() == String::getTypeInfo()){
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

void SQLiteSession::insert(ModelBase& model, bool updateAutoIncPKey){
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
		long lastInsertId = sqlite3_last_insert_rowid((sqlite3*)sessionPtr); //  result.begin()->get(0).convert<long>();
		model.setAutoIncPKey(lastInsertId);
	}
	return;
}

ResultTable SQLiteSession::executeFlat(const QueryBase& query){
	std::string queryString = buildQueryString(query);
	return executeFlat(queryString);
}

ResultTable SQLiteSession::executeFlat(const std::string& query){
	ORMLOG(Logger::Lv::DBUG, "executing query : " + query);
	sqlite3_stmt* stmt = (sqlite3_stmt*)sqliteQuery(query);

	vector<string> columns;
	vector<size_t> columnTypeHashes;

	int num_fields = sqlite3_column_count(stmt);
	for(int i = 0; i < num_fields; i++){
		columns.push_back(sqlite3_column_name(stmt, i));
		columnTypeHashes.push_back(toPrimitiveType(sqlite3_column_type(stmt, i)));
	}

	ResultTable flatResults(columns, columnTypeHashes);

	while(sqlite3_step(stmt) != SQLITE_DONE){
		ORMLOG(Logger::Lv::DBUG, "fetching new row");
		size_t rowIdx = flatResults.addRow();
		for(int i = 0; i < num_fields; i++){
			flatResults.setFieldValue(rowIdx, i, (const char*)sqlite3_column_text(stmt, i));
			//todo: make use of fns like sqlite3_column_int() for better performance
		}
	}

    sqlite3_finalize(stmt);
	return flatResults;
}

std::size_t SQLiteSession::executeVoid(const std::string& query){
	ORMLOG(Logger::Lv::DBUG, "executing query : " + query);
	sqlite3_stmt* stmt = (sqlite3_stmt*)sqliteQuery(query);
	int retval = sqlite3_step(stmt);
    if (retval != SQLITE_DONE && retval != SQLITE_ROW) {
    	//todo: add app specific description to exception msgs, have a help function to wrap it
    	throw runtime_error(sqlite3_errmsg((sqlite3*)sessionPtr));
    }
	return (size_t)sqlite3_changes((sqlite3*)sessionPtr);
}

SQLiteSession::~SQLiteSession() {
	ORMLOG(Logger::Lv::INFO, "disconnected from sqlite database");
	sqlite3_close((sqlite3*)sessionPtr);
}

} /* namespace ORMPlusPlus */
