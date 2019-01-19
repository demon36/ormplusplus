#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include "ModelBase.h"
#include "DB.h"

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

TableColumn& ModelBase::getColumnRef(const std::string& name) const{
	auto column = schemaRef.find(name);
	if(column == schemaRef.end()){
		throw runtime_error("column not found");
	}
	return (*column).second;
}

std::string ModelBase::getTableName() const{
	return tableNameRef;
}

const TableSchema& ModelBase::getSchema() const{
	return schemaRef;
}

void ModelBase::setAttributes(const AttributesMap& values){
	for(auto& attribEntry : this->attributes){
		attribEntry.second = values.at(attribEntry.first);
	}
}

const AttributesMap& ModelBase::getAttributes() const{
	return attributes;
}

bool ModelBase::equals(const ModelBase& that) const{
	for(auto& attribEntry : this->attributes){
		if(!attribEntry.second.equals(that.attributes.at(attribEntry.first))){
			return false;
		}
	}
	return true;
}

void ModelBase::insert(){
	DB::getDefaultSession().insert(*this);
}

string ModelBase::getTypeName(const std::type_info& type){
#ifdef __GNUC__
	int status;
	char* demangled = abi::__cxa_demangle(type.name(),0,0,&status);
	if(status != 0){
		throw runtime_error("failed to demangle class name at ModelBase::getTypeName");
	}
	std::string demangledStr(demangled);
	free(demangled);
	return demangledStr;
#else
	//msvc & clang do not -presumably- mangle class names
	return type.name();
#endif
}

}
