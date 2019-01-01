#include "ModelBase.h"

using namespace std;

namespace ORMPlusPlus{

//void ModelBase::addColumn(std::string name, DataType type){
//	columns[name] = TableColumn();
//	columns[name].setType(type);
//}

//vector<TableColumn> ModelBase::getColumns(){
//	vector<TableColumn> _columns;
//	for(auto& column : columns){
//		_columns.push_back(column.second);
//	}
//	return _columns;
//}

ModelBase::ModelBase(const string& tableName, TableSchema& schema)
: tableNameRef(tableName), schemaRef(schema) {}

ModelBase::ModelBase(const ModelBase& that)
: ModelBase(that.tableNameRef, that.schemaRef)
{
	for(auto& element : that.attributes){
		this->attributes.emplace(element.first, element.second);
	}
}

}
