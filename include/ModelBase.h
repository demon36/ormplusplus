#ifndef INCLUDE_MODELBASE_H_
#define INCLUDE_MODELBASE_H_

#include <vector>
#include <map>

#include "NullableField.h"

namespace ORMPlusPlus{

class NullableFieldBase;

//used for accessing renameColumn() and getColumnNames()
//without having to specify template class parameters
class ModelBase{
private:

protected:


public:
	map<std::string, NullableFieldBase*> fieldValues;
//	static std::vector<TableColumn> getColumns();
//	static void addColumn(std::string name, DataType type);

};
}

#endif /* INCLUDE_MODELBASE_H_ */
