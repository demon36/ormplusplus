#include "DB.h"

namespace ORMPlusPlus{

unique_ptr<Session> DB::session;

void DB::initialize(string host, string database, string user, string password, int port){
	Poco::Data::MySQL::Connector::registerConnector();
	string connectionString = Poco::format("host=%s;port=%d;db=%s;user=%s;password=%s;compress=true;auto-reconnect=true",
			host, port, database, user, password);
	session.reset(new Session("MySQL", connectionString));
}

void DB::executeRawQuery(string query){

}

}
