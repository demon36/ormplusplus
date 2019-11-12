#include "MySQLSession.h"

#include <typeindex>
#include <vector>

#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Tuple.h>

#include "Logger.h"

using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace ORMPlusPlus {

MySQLSession::MySQLSession(const string& host, const string& database, const string& user, const string& password, int port){
	Poco::Data::MySQL::Connector::registerConnector();
	string connectionString = Poco::format("host=%s;port=%d;db=%s;user=%s;password=%s;compress=true;auto-reconnect=true",
			host, port, database, user, password);
	sessionPtr = new Session("MySQL", connectionString);
	ORMLOG(Logger::Lv::INFO, "connected to mysql server " + host + ":" + to_string(port));
}

bool MySQLSession::tableExists(const string& name){
	std::vector<string> foundTables;
	Statement query(*sessionPtr);
	query << "SHOW TABLES LIKE '%s';", into(foundTables), name;
	query.execute();
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

	executeNonQuery(queryStream.str());
}

TableSchema MySQLSession::getTableSchema(const string& name){
	typedef Poco::Tuple<
				string,		//COLUMN_NAME
				string,		//DATA_TYPE
				int,		//CHARACTER_MAXIMUM_LENGTH
				int,		//NUMERIC_PRECISION
				string,		//IS_NULLABLE
				string,		//COLUMN_DEFAULT
				string,		//COLUMN_KEY
				string		//EXTRA
			> DBColumnData;
	std::vector<DBColumnData> rawSchema;
	Statement query(*sessionPtr);
	//TODO: use an ORM class for this ?
	query << "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, NUMERIC_PRECISION, IS_NULLABLE, COLUMN_DEFAULT, COLUMN_KEY, EXTRA "
			"FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s';", into(rawSchema), name;
	query.execute();
	TableSchema schema;
	for(auto& columnTuple : rawSchema){
		string DBColumnType = columnTuple.get<1>();
		size_t typeHash = NullableFieldBase::getTypeHash(DBColumnType);
		string extra = columnTuple.get<7>();
		bool isNullable = columnTuple.get<4>() == "YES";
		bool isPKey = columnTuple.get<6>() == "PRI";
		bool isAutoIncrement = extra.find("auto_increment") != extra.npos;
		TableColumn tempColumn(
				columnTuple.get<0>(),	//COLUMN_NAME
				typeHash,				//DATA_TYPE
				columnTuple.get<2>(),	//CHARACTER_MAXIMUM_LENGTH
				columnTuple.get<3>(),	//NUMERIC_PRECISION
				isNullable,
				columnTuple.get<5>(), 	//COLUMN_DEFAULT
				isPKey,
				isAutoIncrement
		);
		schema.emplace(columnTuple.get<0>(), tempColumn);
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
	if(executeNonQuery(queryStream.str()) != 1){
		throw runtime_error("failed to insert model");
	}

	if(updateAutoIncPKey && model.autoIncPkeyColumnExists()){
		ResultTable result = executeRawQuery("SELECT LAST_INSERT_ID();");
		//const char* test = result.begin()->get(0).type().name();
		//TODO: error check
		long lastInsertId = stol(result.front().begin()->second); //  result.begin()->get(0).convert<long>();
		model.setAutoIncPKey(lastInsertId);
	}
	return;
}

ResultTable MySQLSession::executeFlat(const QueryBase& query){
	std::string queryString = buildQueryString(query);
	ORMLOG(Logger::Lv::DBUG, "executing query : " + queryString);
	Statement st(*sessionPtr);
	st << queryString;
	st.execute();
	RecordSet result = RecordSet(st);
	std::list<std::map<std::string, std::string>> flatResults;

	for(Row& row : result){
		flatResults.push_back({});
		std::map<std::string, std::string>& flatRow = flatResults.back();
		for(size_t colIdx = 0; colIdx < result.columnCount(); colIdx++){
			flatRow[result.columnName(colIdx)] = row[result.columnName(colIdx)].toString();//TODO: optimize
		}
	}

	return flatResults;
}

ResultTable MySQLSession::executeRawQuery(const std::string& queryString){
	ORMLOG(Logger::Lv::DBUG, "executing query : " + queryString);
	Statement st(*sessionPtr);
	st << queryString;
	st.execute();
	RecordSet result = RecordSet(st);
	std::list<std::map<std::string, std::string>> flatResults;
	for(Row& row : result){
		flatResults.push_back({});
		std::map<std::string, std::string>& flatRow = flatResults.back();
		for(const string& colName : *row.names()){
			flatRow[colName] = row[colName].toString();
		}
	}
	return flatResults;
}

std::size_t MySQLSession::executeNonQuery(const std::string& queryString){
	ORMLOG(Logger::Lv::DBUG, "executing query : " + queryString);
	Statement query(*sessionPtr);
	query << queryString;
	return query.execute();
}

MySQLSession::~MySQLSession() {
	ORMLOG(Logger::Lv::INFO, "disconnected from mysql server ");
	delete sessionPtr;
}

} /* namespace ORMPlusPlus */
