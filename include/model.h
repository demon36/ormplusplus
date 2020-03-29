#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "attrib_initializer.h"
#include "model_base.h"
#include "query.h"

//TODO: facilitate writing conditions using sth like:
//#define DEFINE_ATTRIB(DATATYPE, NAME) static std::string _ ## NAME(){static std::string _s = #NAME; return _s;}; DATATYPE NAME = initialize_attrib<DATATYPE>(#NAME)
//or a static reference that is initialized to a schema entry
//this is better static const table_column& ID_(){ return schema[""]; };
//so that queries can like where({my_model::_ID(), "=", 3})
#define DEFINE_FIELD(DATATYPE, NAME) DATATYPE NAME = initialize_attrib(#NAME)

namespace ormplusplus{

template<class user_model>
class model : public model_base
{
private:
protected:
	static std::string table_name_;
	static table_schema schema;
	//set to true after first instance get created
	static volatile bool schema_built;
	constexpr static const char* table_name = nullptr;

	model<user_model>(model<user_model>&& that)
	: model_base(that)
	{
	}
public:

	model<user_model>()
	: model_base(table_name_, schema)
	{
		schema_built = true;
	}

	model<user_model>(const model<user_model>& that) = delete;


	static void copy(const model<user_model>& src, model<user_model>& dest){
		dest.set_attribs(src.attributes);
	}

	user_model clone(){
		user_model obj;
		copy(*this, obj);
		return obj;
	}

	static std::vector<user_model> clone(const std::vector<user_model>& source){
		std::vector<user_model> target(source.size());
		for(int i = 0; i < source.size(); i++){
			copy(source[i], target[i]);
		}
		return target;
	}

	model<user_model>& operator=(model<user_model>& that){
		this->attributes = that.attributes;
		return *this;
	}

	static std::string get_table_name(){
		return table_name_;
	}

	static query<user_model, query_type::_select_single> select_one(){
		return {};
	}

	static query<user_model, query_type::_select_many> select_many(){
		return {};
	}

	static query<user_model, query_type::_delete> delete_(){
		return {};
	}

	static query<user_model, query_type::_update> update(){
		return {};
	}

	//TODO: add another variant that takes no template parameters and performs the with_default() check in runtime
	//		this will eliminate the need for macros
	//TODO: should this fn be renamed to bind_attribute ?
	attrib_initializer initialize_attrib(const std::string& name){
		//todo: do not pass schema if already initialized or column has been already added
		//todo: reduce args, probably no need to pass the whole schema and the whole model_base
		//todo: check of col is already added
		return {name, get_schema(), *this};

		/*
		if(column_already_added){
			return {field, nullptr};
		}else{
			return {field, &schema[name]};
		}
		*/
	}

	static bool col_exists(const std::string& name) {
		return get_schema().find(name) != get_schema().end();
	}

	//todo: is this necessary ?
	static table_schema& get_schema() {
		//built schema by creating a model instance before processing the query
		if(!schema_built){
			user_model dummy_instance;
		}
		return schema;
	}

	virtual ~model(){}
		
};

template<class user_model>
table_schema model<user_model>::schema;

template<class user_model>
std::string model<user_model>::table_name_ =
		user_model::table_name == nullptr ?
		type_info::get_type_name(typeid(user_model)) : user_model::table_name;

template<class user_model>
volatile bool model<user_model>::schema_built = false;

}

#endif //MODEL_H
