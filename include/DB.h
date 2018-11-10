#ifndef INCLUDE_DBH_
#define INCLUDE_DBH_

#include <string>
#include <memory>

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>

class ModelBase;

#define MYSQL_DEFAULT_PORT 3306

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data;

namespace ORMPlusPlus{

class DB{

public:
	static std::unique_ptr<Session> session;
	static void initialize(std::string host, std::string database, std::string user, std::string password, int port = MYSQL_DEFAULT_PORT);
	static void createOrReplaceTable(ModelBase& userModel);
	static void createTable(ModelBase& userModel);
	static bool checkTableExists(ModelBase& userModel, bool checkSchema = false);
	static void executeRawQuery(std::string query);
};

}



#endif /* INCLUDE_DBH_ */
