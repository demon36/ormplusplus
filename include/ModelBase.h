#ifndef INCLUDE_MODELBASE_H_
#define INCLUDE_MODELBASE_H_

#include <vector>
#include <map>

#include "NullableField.h"

using namespace std;

namespace ORMPlusPlus{
//used for accessing renameColumn() and getColumnNames()
//without having to specify template class parameters
class ModelBase{
protected:
	//holds attribute name as key
	//NullableFields ptr as value
	map<string, NullableFieldBase*> fields;
public:
	std::vector<string> getColumnNames(){
		std::vector<string> columnNames;
		for(auto& field : fields){
			columnNames.push_back(field.first);
		}
		return columnNames;
	}
};

}

#endif /* INCLUDE_MODELBASE_H_ */
