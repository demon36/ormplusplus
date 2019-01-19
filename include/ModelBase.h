#ifndef INCLUDE_MODELBASE_H_
#define INCLUDE_MODELBASE_H_

#include <vector>
#include <map>
#include <memory>

#include "TableColumn.h"
#include "NullableFieldBase.h"

namespace ORMPlusPlus{

typedef std::map<std::string, NullableFieldBase> AttributesMap;

class ModelBase{
	friend class DBSessionBase;
private:
	TableSchema& schemaRef;//owner is always Model<T, U>
	const std::string& tableNameRef;
protected:
	AttributesMap attributes;
public:
	ModelBase(const std::string& tableName, TableSchema& schema);
	ModelBase(const ModelBase& that);
	void addColumn(const std::string& name, std::size_t typeHash);
	TableColumn& getColumnRef(const std::string& name) const;
	std::string getTableName() const;
	const TableSchema& getSchema() const;
	void setAttributes(const AttributesMap& values);
	const AttributesMap& getAttributes() const;
	bool equals(const ModelBase& that) const;
	/**
	 * inserts a new row or updates existing
	 */
//	void save();
//	void load();
	void insert();
//	void update();
//	static std::vector<TableColumn> getColumns();
//	static void addColumn(std::string name, DataType type);
	//TODO: implement
	std::string toString();
	static std::string getTypeName(const std::type_info& type);
};
}

#endif /* INCLUDE_MODELBASE_H_ */
