#ifndef INCLUDE_MYSQLSESSION_H_
#define INCLUDE_MYSQLSESSION_H_

#include <string>

#include "DBSessionBase.h"
#include "ResultTable.h"

#define MYSQL_DEFAULT_PORT 3306

namespace ORMPlusPlus {

class MySQLSession : public DBSessionBase {
private:
	void* sessionPtr;
	void mysqlQuery(const std::string& query);
	static size_t toPrimitiveType(int mySQLTypeEnum);
	static const TypeInfo& getTypeInfo(const std::string& mySQLColTypeName);
	static const std::map<std::string, TypeInfo> typeNamesMap;
public:
	MySQLSession(const std::string& host, const std::string& database, const std::string& user, const std::string& password, int port = MYSQL_DEFAULT_PORT);
	bool tableExists(const std::string& name) override;
	void createTable(const std::string& name, const TableSchema& schema) override;
	TableSchema getTableSchema(const std::string& name) override;
	void insert(ModelBase& model, bool updateAutoIncPKey = false) override;
	ResultTable executeFlat(const QueryBase& query) override;
	ResultTable executeFlat(const std::string& query) override;
	std::size_t executeVoid(const std::string& query) override;
	virtual ~MySQLSession();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_MYSQLSESSION_H_ */
