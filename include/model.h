#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "attrib_initializer.h"
#include "typestring.hh"

#include "model_base.h"
#include "query.h"

//TODO: get rid of model binding macros
//macro overloading
#define GET_MACRO(_1,_2,NAME,...) NAME
#define BOUND_MODEL(...) GET_MACRO(__VA_ARGS__, BOUND_MODEL_2, BOUND_MODEL_1)(__VA_ARGS__)

#define BOUND_MODEL_1(CLASS_NAME) struct CLASS_NAME : public ormplusplus::model<CLASS_NAME>
#define BOUND_MODEL_2(CLASS_NAME, TABLE_NAME) struct CLASS_NAME : public ormplusplus::model<CLASS_NAME, typestring_is(TABLE_NAME)>

//creates a set of template parameters from a string literal
#define _ typestring_is
//TODO: facilitate writing conditions using sth like:
//#define DEFINE_ATTRIB(DATATYPE, NAME) static std::string _ ## NAME(){static std::string _s = #NAME; return _s;}; DATATYPE NAME = initialize_attrib<DATATYPE>(#NAME)
//or a static reference that is initialized to a schema entry
//this is better static const table_column& ID_(){ return schema[""]; };
//so that queries can like where({my_model::_ID(), "=", 3})
#define DEFINE_ATTRIB(DATATYPE, NAME) DATATYPE NAME = initialize_attrib<DATATYPE>(#NAME)

namespace ormplusplus{

template<class user_model, class table_name = typestring_is("")>
class model : public model_base
{
private:
protected:
	static std::string table_name_;
	static table_schema schema;
	//set to true after first instance get created
	static bool schema_built;

	/**
	 * adds a column to model schema
	 * @return true at success, false if already exists
	 */
	static bool add_col_if_not_exists(const std::string& name, const type_info& type){
		if(col_exists(name)){
			return false;
		}else{
			schema.emplace(name, table_column(name, type));
			return true;
		}
	}
public:

	model<user_model, table_name>()
	: model_base(table_name_, schema)
	{
		schema_built = true;
	}

	model<user_model, table_name>(const model<user_model, table_name>& that) = delete;
	model<user_model, table_name>(model<user_model, table_name>&& that) = default;

	static void copy(const model<user_model, table_name>& src, user_model& dest){
		dest.set_attribs(src.attributes);
	}

	user_model clone(){
		user_model obj;
		copy(*this, obj);
		return obj;
	};

	static std::vector<user_model> clone(const std::vector<user_model>& source){
		std::vector<user_model> target(source.size());
		for(int i = 0; i < source.size(); i++){
			copy(source[i], target[i]);
		}
		return target;
	}

	model<user_model, table_name>& operator=(model<user_model, table_name>& that){
		this->attributes = that.attributes;
		return *this;
	}

	static std::vector<user_model> get(){
		query<user_model> query;
		return query.get();
	}

	static std::string get_table_name(){
		return table_name_;
	}

	//TODO: implement nested conditions
	static query<user_model> where(std::vector<query_condition> conditions){
		return query<user_model>(conditions);
	}

	static query<user_model> where(query_condition condition){
		return query<user_model>({condition});
	}

	//TODO: add another variant that takes no template parameters and performs the with_default() check in runtime
	//		this will eliminate the need for macros
	//TODO: should this fn be renamed to bind_attribute ?
	template<typename attrib_type>
	attrib_initializer<attrib_type> initialize_attrib(const std::string& name){
		bool column_already_added = !add_col_if_not_exists(name, attrib_type::get_type_info());
		//TODO: move logic to model_base
		nullable_field_base& field = attributes.emplace(name, nullable_field_base(attrib_type::get_type_info())).first->second;
		if(column_already_added){
			return {field, nullptr};
		}else{
			return {field, &schema[name]};
		}
	}

	static bool col_exists(std::string name) {
		return get_schema().find(name) != get_schema().end();
	}

	//todo: is this necessary ?
	static const table_schema& get_schema() {
		//built schema by creating a model instance before processing the query
		if(!schema_built){
			user_model dummy_instance;
		}
		return schema;
	}
		
};

template<class user_model, class table_name>
table_schema model<user_model, table_name>::schema;

template<class user_model, class table_name>
std::string model<user_model, table_name>::table_name_ =
		std::string(table_name::data()).empty() ?
		type_info::get_type_name(typeid(user_model)) : table_name::data();

template<class user_model, class table_name>
bool model<user_model, table_name>::schema_built = false;

}

#endif //MODEL_H
