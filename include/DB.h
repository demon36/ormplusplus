#ifndef INCLUDE_DBH_
#define INCLUDE_DBH_

#include <string>
#include <memory>
#include <type_traits>
#include <exception>

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Tuple.h>

#include <TableColumn.h>

#define MYSQL_DEFAULT_PORT 3306

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace ORMPlusPlus{

template<class UserModel, const char* TableName>
class Model;

class DB{
private:
	static std::unique_ptr<Session> session;
public:
	//make sure template parameter is a child of Model<UserModel>
	//FIXME: this function has no effect since UserModel is no longer a child of ModelBase
	template <class UserModel>
	static void assertClassIsUserModel(){
		return;
		if(!std::is_base_of<ModelBase, UserModel>::value){
			throw std::runtime_error("used class that is not a legitimate child of Model<class UserModel>");
		}
	}

	static Session& getSession(){
		if(session != nullptr){
			return *session.get();
		}else{
			throw std::runtime_error("DB::getSession() called with no session set");
		}
	}

	static void initialize(std::string host, std::string database, std::string user, std::string password, int port = MYSQL_DEFAULT_PORT);

	template <class UserModel>
	static void createOrReplaceTable(){

	}

	// CREATE TABLE 'client_info' ('age' INT, 'height' INT, 'name' VARCHAR(1024));
	template <class UserModel>
	static void createTable(){
		assertClassIsUserModel<UserModel>();
		Statement query(getSession());
		query << "CREATE TABLE IF NOT EXISTS `%s`(", UserModel::getTableName();
		TableSchema schema = UserModel::getColumnDefs();
		std::vector<TableColumn> columnsList;
	    for( TableSchema::iterator it = schema.begin(); it != schema.end(); ++it ) {
	    	columnsList.push_back( it->second );
	    }

		std::string primaryKeys;
		for(int i = 0; i < columnsList.size(); i++){
			if(columnsList[i].isPrimary()){
				if(!primaryKeys.empty()){
					primaryKeys += ",";
				}
				primaryKeys += columnsList[i].getName();
			}
			query << "`%s` %s", columnsList[i].getName(), columnsList[i].getTypeName();
			if(columnsList[i].getType() == DataType::_String){
				query << "(%d)", columnsList[i].getLength();
			}
			if(i < columnsList.size()-1){
				query << ", ";
			}
		}

		if(!primaryKeys.empty()){
			query << " PRIMARY KEY (%s) ", primaryKeys;
		}
		query << ");";
		query.execute();
	}

	template <class UserModel>
	static void dropTable(){
		Statement query(getSession());
		query << "DROP TABLE IF EXISTS `%s`;", UserModel::getTableName();
		query.execute();
	}

	static bool tableNameExists(std::string name){
		std::vector<std::string> foundTables;
		Statement query(getSession());
		query << "SHOW TABLES LIKE '%s';", into(foundTables), name;
		query.execute();
		return !foundTables.empty();
	}

	static TableSchema getTableSchema(std::string name){
		typedef Poco::Tuple<
					std::string,	//COLUMN_NAME
					std::string,	//DATA_TYPE
					int,			//CHARACTER_MAXIMUM_LENGTH
					int,			//NUMERIC_PRECISION
					bool,			//IS_NULLABLE
					std::string,	//COLUMN_DEFAULT
					bool			//COLUMN_KEY
				> DBColumnData;
		std::vector<DBColumnData> rawSchema;
		Statement query(getSession());
		query << "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, NUMERIC_PRECISION, IS_NULLABLE, COLUMN_DEFAULT, COLUMN_KEY "
				"FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s';", into(rawSchema), name;
		query.execute();
		TableSchema schema;
		for(auto& columnTuple : rawSchema){
			DataType tempType;
			std::string DBColumnType = columnTuple.get<1>();
			if(DBColumnType == "int"){
				tempType = DataType::_Integer;
			}else if(DBColumnType == "varchar"){
				tempType = DataType::_String;
			}else{
				tempType = DataType::_UnsupportedType;
			}
			TableColumn tempColumn(
					columnTuple.get<0>(),	//COLUMN_NAME
					tempType,				//DATA_TYPE
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

	template <class UserModel>
	static bool tableExists(bool checkSchema = false){
		assertClassIsUserModel<UserModel>();
		std::string tableName = UserModel::getTableName();
		TableSchema providedSchema = UserModel::getColumnDefs();
		if(checkSchema){
			auto foundSchema = getTableSchema(tableName);
			for(auto& column : providedSchema){
				auto& foundColumn = foundSchema[column.first];
				if(column.second != foundColumn){
					return false;
				}
				if(column.second.getType() == DataType::_Integer &&
					column.second.getPrecision() > foundColumn.getPrecision()){
					return false;
				}
				if(column.second.getType() == DataType::_String &&
					column.second.getLength() > foundColumn.getLength()){
					return false;
				}
			}
			return true;
		}else{
			return tableNameExists(tableName);
		}
	}

	static Statement::Result executeRawQuery(std::string query);
};

}



#endif /* INCLUDE_DBH_ */
