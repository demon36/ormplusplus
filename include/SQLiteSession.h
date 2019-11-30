#ifndef INCLUDE_SQLITESESSION_H_
#define INCLUDE_SQLITESESSION_H_

#include <string>

#include "DBSessionBase.h"
#include "ResultTable.h"

namespace ORMPlusPlus {

class SQLiteSession : public DBSessionBase {
private:
	void* sessionPtr;
	void* sqliteQuery(const std::string& query);
	static size_t toPrimitiveType(int mySQLTypeEnum);
	static const TypeInfo& getTypeInfo(const std::string& mySQLColTypeName);
	static const std::map<std::string, TypeInfo> typeNamesMap;
public:
	SQLiteSession(const std::string& dbPath);
	bool tableExists(const std::string& name) override;
	void createTable(const std::string& name, const TableSchema& schema) override;
	TableSchema getTableSchema(const std::string& name) override;
	void insert(ModelBase& model, bool updateAutoIncPKey = false) override;
	ResultTable executeFlat(const QueryBase& query) override;
	ResultTable executeFlat(const std::string& query) override;
	std::size_t executeVoid(const std::string& query) override;
	virtual ~SQLiteSession();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_SQLITESESSION_H_ */
