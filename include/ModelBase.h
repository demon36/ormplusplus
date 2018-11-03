#ifndef INCLUDE_MODELBASE_H_
#define INCLUDE_MODELBASE_H_

#include <vector>
#include <map>

#include "NullableField.h"

namespace ORMPlusPlus{

//pair<name, type>
typedef pair<std::string, DataType> TableColumn;

class NullableFieldBase;

//used for accessing renameColumn() and getColumnNames()
//without having to specify template class parameters
class ModelBase{
protected:
	//map<column name, value>
	map<std::string, NullableFieldBase*> fieldValues;
	static std::vector<TableColumn> columns;
public:
	static std::vector<TableColumn> getColumns();
};
}

#endif /* INCLUDE_MODELBASE_H_ */
