#include "ModelBase.h"

using namespace std;

namespace ORMPlusPlus{

	vector<TableColumn> ModelBase::columns;
	vector<TableColumn> ModelBase::getColumns(){
		vector<TableColumn> _columns;
		for(auto& column : columns){
			_columns.push_back({column.first, column.second});
		}
		return _columns;
	}


}
