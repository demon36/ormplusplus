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
	for(const auto& attribEntry : this->attributes){
		if(!attribEntry.second.equals(that.attributes.at(attribEntry.first))){
			return false;
		}
	}
	return true;
}

bool ModelBase::autoIncPkeyColumnExists() const{
	for(const auto& column : schemaRef){
		if(column.second.isAutoIncrement() && column.second.isPrimary()){
			return true;
		}
	}
	return false;
}

void ModelBase::setAutoIncPKey(long value){
	TableColumn* columnPtr = nullptr;
	for(auto& column : schemaRef){
		if(column.second.isAutoIncrement() && column.second.isPrimary()){
			//TODO: check for multiple auto increment primary keys
			columnPtr = &column.second;
			break;
		}
	}
	if(columnPtr == nullptr){
		throw runtime_error("not auto increment primary key found");
	}else{
		//TODO: think of a cleaner way
		//TODO: add tests for all types
		if(columnPtr->getTypeInfo().nullableTypeHash == typeid(Integer).hash_code()){
			attributes.at(columnPtr->getName()).setValue<int>((int)value);
		}else if(columnPtr->getTypeInfo().nullableTypeHash == typeid(Long).hash_code()){
			attributes.at(columnPtr->getName()).setValue<long>((long)value);
		}else if(columnPtr->getTypeInfo().nullableTypeHash == typeid(Float).hash_code()){
			attributes.at(columnPtr->getName()).setValue<float>((float)value);
		}else if(columnPtr->getTypeInfo().nullableTypeHash == typeid(Double).hash_code()){
			attributes.at(columnPtr->getName()).setValue<double>((double)value);
		}else{
			throw runtime_error("unsupported data type used for auto increment primary key");
		}
	}
}

void ModelBase::insert(bool updateAutoIncPKey){
	DB::getDefaultSession().insert(*this, updateAutoIncPKey);
}

}
