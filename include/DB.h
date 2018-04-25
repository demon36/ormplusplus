#ifndef INCLUDE_DBH_
#define INCLUDE_DBH_

#include <string>

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>

#include "ormplusplus.h"

#define MYSQL_DEFAULT_PORT 3306

using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data;

namespace ORMPlusPlus{

class DB{
	static unique_ptr<Session> session;
public:
	static void initialize(string host, string database, string user, string password, int port = MYSQL_DEFAULT_PORT);
	static void executeRawQuery(string query);
};

}



#endif /* INCLUDE_DBH_ */
