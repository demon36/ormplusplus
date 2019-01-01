#ifndef INCLUDE_MODELBASE_H_
#define INCLUDE_MODELBASE_H_

#include <vector>
#include <map>
#include <memory>

#include "TableColumn.h"

namespace ORMPlusPlus{

class ModelBase{
private:
	TableSchema& schemaRef;
	const std::string& tableNameRef;
protected:
	std::map<std::string, NullableFieldBase> attributes;
public:
	ModelBase(const std::string& tableName, TableSchema& schema);
	ModelBase(const ModelBase& that);
//	std::map<std::string, NullableFieldBase*> fieldValues;
//	static std::vector<TableColumn> getColumns();
//	static void addColumn(std::string name, DataType type);

};
}

#endif /* INCLUDE_MODELBASE_H_ */
