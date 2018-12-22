#ifndef INCLUDE_DBSESSIONBASE_H_
#define INCLUDE_DBSESSIONBASE_H_

#include <memory>

#include "TableColumn.h"
#include "QueryBase.h"

namespace ORMPlusPlus {

class DBSessionBase {
public:
	DBSessionBase();
	void createTable(const std::string& name, const TableSchema& schema);
	void dropTable(const std::string& name);
	virtual bool tableExists(const std::string& name) = 0;
	bool tableExists(const std::string& name, TableSchema& schema);
	virtual TableSchema getTableSchema(const std::string& name) = 0;
	std::map<std::string, std::unique_ptr<NullableFieldBase>> select(const QueryBase& query);
	virtual void execute(const std::string& query) = 0;
	virtual ~DBSessionBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_DBSESSIONBASE_H_ */
