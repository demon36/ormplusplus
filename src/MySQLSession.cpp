#include "MySQLSession.h"

#include <typeindex>

#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Tuple.h>

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
}

bool MySQLSession::tableExists(const string& name){
	std::vector<string> foundTables;
	Statement query(*sessionPtr);
	query << "SHOW TABLES LIKE '%s';", into(foundTables), name;
	query.execute();
	return !foundTables.empty();
}

TableSchema MySQLSession::getTableSchema(const string& name){
	typedef Poco::Tuple<
				string,	//COLUMN_NAME
				string,	//DATA_TYPE
				int,			//CHARACTER_MAXIMUM_LENGTH
				int,			//NUMERIC_PRECISION
				bool,			//IS_NULLABLE
				string,	//COLUMN_DEFAULT
				bool			//COLUMN_KEY
			> DBColumnData;
	std::vector<DBColumnData> rawSchema;
	Statement query(*sessionPtr);
	query << "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, NUMERIC_PRECISION, IS_NULLABLE, COLUMN_DEFAULT, COLUMN_KEY "
			"FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s';", into(rawSchema), name;
	query.execute();
	TableSchema schema;
	for(auto& columnTuple : rawSchema){
		type_index test = typeid(int);
		//TODO: is this safe ?
		const type_info* typeIdxPtr = nullptr;
		string DBColumnType = columnTuple.get<1>();
		if(DBColumnType == "int"){
			typeIdxPtr = &typeid(int);
		}else if(DBColumnType == "varchar"){
			typeIdxPtr = &typeid(String);
		}else{
			throw runtime_error("unsupported database type");
		}
		TableColumn tempColumn(
				columnTuple.get<0>(),	//COLUMN_NAME
				*typeIdxPtr,				//DATA_TYPE
				columnTuple.get<2>(),	//CHARACTER_MAXIMUM_LENGTH
				columnTuple.get<3>(),	//NUMERIC_PRECISION
				columnTuple.get<4>(), 	//IS_NULLABLE
				columnTuple.get<5>(), 	//COLUMN_DEFAULT
				columnTuple.get<6>()	//COLUMN_KEY
		);
		schema.emplace(columnTuple.get<0>(), tempColumn);
	}
	return schema;
}

void MySQLSession::execute(const string& queryString){
	Statement query(*sessionPtr);
	query << queryString;
	query.execute();
}

MySQLSession::~MySQLSession() {
	delete sessionPtr;
}

} /* namespace ORMPlusPlus */
