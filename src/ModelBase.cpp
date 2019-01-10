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
: tableNameRef(tableName), schemaRef(schema)
{
}

ModelBase::ModelBase(const ModelBase& that)
: ModelBase(that.tableNameRef, that.schemaRef)
{
	for(auto& element : that.attributes){
		this->attributes.emplace(element.first, element.second);
	}
}

void ModelBase::addColumn(const std::string& name, std::size_t typeHash){
	schemaRef.emplace(name, TableColumn(name, typeHash));
}

TableColumn& ModelBase::getColumnRef(const std::string& name){
	auto column = schemaRef.find(name);
	if(column == schemaRef.end()){
		throw runtime_error("column not found");
	}
	return (*column).second;
}

void ModelBase::setAttributes(const AttributesMap& values){
	for(auto& attribEntry : this->attributes){
		attribEntry.second = values.at(attribEntry.first);
	}
}
}
