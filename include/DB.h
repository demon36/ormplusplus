#ifndef INCLUDE_DBH_
#define INCLUDE_DBH_

#include <string>
#include <memory>
#include <type_traits>
#include <exception>

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>

#define MYSQL_DEFAULT_PORT 3306

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace ORMPlusPlus{

template<class UserModel>
class Model;

class DB{
private:
	static std::unique_ptr<Session> session;
public:
	//make sure template parameter is a child of Model<UserModel>
	template <class UserModel>
	static void assertClassIsUserModel(){
		if(!std::is_base_of<Model<UserModel>, UserModel>::value){
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

	template <class UserModel>
	static void createTable(){

	}

	template <class UserModel>
	static void dropTable(){

	}

	template <class UserModel>
	static bool tableExists(bool checkSchema = false){
		assertClassIsUserModel<UserModel>();
		std::string tableName = UserModel::getTableName();
		std::vector<std::string> foundTables;
		Statement query = (getSession() << "SHOW TABLES LIKE '%s';", into(foundTables), tableName);
		query.execute();
		return !foundTables.empty();
	}

	static Statement::Result executeRawQuery(std::string query);
};

}



#endif /* INCLUDE_DBH_ */
