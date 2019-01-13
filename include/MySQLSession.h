#ifndef INCLUDE_MYSQLSESSION_H_
#define INCLUDE_MYSQLSESSION_H_

#include <string>

#include <Poco/Data/Session.h>

#include "DBSessionBase.h"
#include "Logger.h"

#define MYSQL_DEFAULT_PORT 3306

namespace ORMPlusPlus {

class MySQLSession : public DBSessionBase {
private:
	Poco::Data::Session* sessionPtr;
public:
	MySQLSession(const std::string& host, const std::string& database, const std::string& user, const std::string& password, int port = MYSQL_DEFAULT_PORT);
	bool tableExists(const std::string& name);
	virtual void createTable(const std::string& name, const TableSchema& schema);
	TableSchema getTableSchema(const std::string& name);
	Poco::Data::RecordSet execute(const std::string& queryString);
	virtual ~MySQLSession();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_MYSQLSESSION_H_ */
