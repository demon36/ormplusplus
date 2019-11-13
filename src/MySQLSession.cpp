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
	return !foundTables.empty();
}

void MySQLSession::createTable(const string& name, const TableSchema& schema){
	stringstream queryStream;
	queryStream << "CREATE TABLE IF NOT EXISTS `"<< name <<"`(";
	std::vector<TableColumn> columnsList;
	for(auto& columnEntry : schema ) {
		columnsList.push_back( columnEntry.second );
	}

	for(size_t i = 0; i < columnsList.size(); i++){
		queryStream << "`"<< columnsList[i].getName() <<"` " <<  columnsList[i].getDBTypeName();
		if(columnsList[i].isText()){
			queryStream << "(" << columnsList[i].getLength() << ")";
		}
		if(columnsList[i].isAutoIncrement()){
			queryStream << " AUTO_INCREMENT ";
		}
		if(!columnsList[i].getDefaultValue().empty()){
			queryStream << " DEFAULT '" << columnsList[i].getDefaultValue() << "' ";
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
	for(auto& row : result){
		string DBColumnType = row["DATA_TYPE"];
		size_t typeHash = NullableFieldBase::getTypeHash(DBColumnType);
		string extra = row["EXTRA"];
		//todo: find better soln other than converting null to -1
		int maxLength = row["CHARACTER_MAXIMUM_LENGTH"].empty() ? -1 : stoi(row["CHARACTER_MAXIMUM_LENGTH"]);
		int numPrecision = row["NUMERIC_PRECISION"].empty() ? -1 : stoi(row["NUMERIC_PRECISION"]);
		bool isNullable = row["IS_NULLABLE"] == "YES";
		bool isPKey = row["COLUMN_KEY"] == "PRI";
		bool isAutoIncrement = extra.find("auto_increment") != extra.npos;
		TableColumn tempColumn(
				row["COLUMN_NAME"],
				typeHash,
				maxLength,
				numPrecision,
				isNullable,
				row["COLUMN_DEFAULT"],
				isPKey,
				isAutoIncrement
		);
		schema.emplace(row["COLUMN_NAME"], tempColumn);
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
		long lastInsertId = stol(result.front().begin()->second); //  result.begin()->get(0).convert<long>();
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
	vector<string> columns;//todo: optimize and remove this
	std::list<std::map<std::string, std::string>> flatResults;
	int num_fields = mysql_num_fields(result);

	while((field = mysql_fetch_field(result)) != NULL){
		columns.push_back(field->name);
	}

	while((row = mysql_fetch_row(result)) != NULL){
		flatResults.push_back({});
		std::map<std::string, std::string>& flatRow = flatResults.back();
		for(int i = 0; i < num_fields; i++){
			flatRow[columns[i]] = row[i] ? row[i] : "";
			//todo: differentiate between NULL, "NULL", empty values
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
