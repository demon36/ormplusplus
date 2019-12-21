#include <db.h>
#include "model_base.h"

using namespace std;

namespace ormplusplus{

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

model_base::model_base(const string& tableName, table_schema& schema)
: table_name_ref(tableName), schema_ref(schema)
{
}

model_base::model_base(const model_base& that)
: model_base(that.table_name_ref, that.schema_ref)
{
	for(auto& element : that.attributes){
		this->attributes.emplace(element.first, element.second);
	}
}

table_column& model_base::get_col_ref(const std::string& name) const{
	auto column = schema_ref.find(name);
	if(column == schema_ref.end()){
		throw runtime_error("column not found");
	}
	return (*column).second;
}

std::string model_base::get_table_name() const{
	return table_name_ref;
}

const table_schema& model_base::get_schema() const{
	return schema_ref;
}

void model_base::set_attribs(const attribs_map& values){
	for(auto& attribEntry : this->attributes){
		attribEntry.second = values.at(attribEntry.first);
	}
}

const attribs_map& model_base::get_attribs() const{
	return attributes;
}

bool model_base::equals(const model_base& that) const{
	for(const auto& attribEntry : this->attributes){
		if(!attribEntry.second.equals(that.attributes.at(attribEntry.first))){
			return false;
		}
	}
	return true;
}

bool model_base::auto_inc_pkey_col_exists() const{
	for(const auto& column : schema_ref){
		if(column.second.is_auto_increment() && column.second.is_primary_key()){
			return true;
		}
	}
	return false;
}

void model_base::set_auto_inc_pkey(long value){
	table_column* columnPtr = nullptr;
	for(auto& column : schema_ref){
		if(column.second.is_auto_increment() && column.second.is_primary_key()){
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
		if(columnPtr->get_type_info().wrapper_type_hash == typeid(Integer).hash_code()){
			attributes.at(columnPtr->get_name()).setValue<int>((int)value);
		}else if(columnPtr->get_type_info().wrapper_type_hash == typeid(Long).hash_code()){
			attributes.at(columnPtr->get_name()).setValue<long>((long)value);
		}else if(columnPtr->get_type_info().wrapper_type_hash == typeid(Float).hash_code()){
			attributes.at(columnPtr->get_name()).setValue<float>((float)value);
		}else if(columnPtr->get_type_info().wrapper_type_hash == typeid(Double).hash_code()){
			attributes.at(columnPtr->get_name()).setValue<double>((double)value);
		}else{
			throw runtime_error("unsupported data type used for auto increment primary key");
		}
	}
}

void model_base::insert(bool updateAutoIncPKey){
	db::get_default_session().insert(*this, updateAutoIncPKey);
}

}
