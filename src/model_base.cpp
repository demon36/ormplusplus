#include <db.h>
#include "model_base.h"

using namespace std;

namespace ormplusplus{

//void model_base::add_column(std::string name, data_type type){
//	columns[name] = table_column();
//	columns[name].set_type(type);
//}

//vector<Table_column> model_base::get_columns(){
//	vector<table_column> _columns;
//	for(auto& column : columns){
//		_columns.push_back(column.second);
//	}
//	return _columns;
//}

model_base::model_base(const string& table_name, table_schema& schema)
: table_name_ref(table_name), schema_ref(schema)
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
	for(auto& attrib_entry : this->attributes){
		attrib_entry.second = values.at(attrib_entry.first);
	}
}

const attribs_map& model_base::get_attribs() const{
	return attributes;
}

bool model_base::equals(const model_base& that) const{
	for(const auto& attrib_entry : this->attributes){
		if(!attrib_entry.second.equals(that.attributes.at(attrib_entry.first))){
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
	table_column* column_ptr = nullptr;
	for(auto& column : schema_ref){
		if(column.second.is_auto_increment() && column.second.is_primary_key()){
			//TODO: check for multiple auto increment primary keys
			column_ptr = &column.second;
			break;
		}
	}
	if(column_ptr == nullptr){
		throw runtime_error("not auto increment primary key found");
	}else{
		//TODO: think of a cleaner way
		//TODO: add tests for all types
		if(column_ptr->get_type_info().wrapper_type_hash == typeid(db_int).hash_code()){
			attributes.at(column_ptr->get_name()).set_value<int>((int)value);
		}else if(column_ptr->get_type_info().wrapper_type_hash == typeid(db_long).hash_code()){
			attributes.at(column_ptr->get_name()).set_value<long>((long)value);
		}else if(column_ptr->get_type_info().wrapper_type_hash == typeid(db_float).hash_code()){
			attributes.at(column_ptr->get_name()).set_value<float>((float)value);
		}else if(column_ptr->get_type_info().wrapper_type_hash == typeid(db_double).hash_code()){
			attributes.at(column_ptr->get_name()).set_value<double>((double)value);
		}else{
			throw runtime_error("unsupported data type used for auto increment primary key");
		}
	}
}

void model_base::insert(bool update_auto_inc_pkey){
	db::get_default_session().insert(*this, update_auto_inc_pkey);
}

}
