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
	std::map<std::string, std::unique_ptr<NullableFieldBase>> attributes;
public:
	ModelBase(TableSchema& schema, const std::string& tableName);
//	std::map<std::string, NullableFieldBase*> fieldValues;
//	static std::vector<TableColumn> getColumns();
//	static void addColumn(std::string name, DataType type);

};
}

#endif /* INCLUDE_MODELBASE_H_ */
