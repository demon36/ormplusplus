#ifndef INCLUDE_DB_SESSION_BASE_H_
#define INCLUDE_DB_SESSION_BASE_H_

#include "table_column.h"
#include "query.h"
#include "model_base.h"
#include "result_table.h"

#include "iostream"//todo: remove after testing
namespace ormplusplus {

class db_session_base {
public:
	template <class user_model>
	void create_table(){
		model_base::assert_class_is_user_model<user_model>();
		create_table(user_model::get_table_name(), user_model::get_schema());
	}

	virtual void create_table(const std::string& name, const table_schema& schema) = 0;

	template <class user_model>
	void drop_table(){
		drop_table(user_model::get_table_name());
	}

	void drop_table(const std::string& name);

	template <class user_model>
	bool table_exists(bool check_schema = false){
		model_base::assert_class_is_user_model<user_model>();
		table_schema schema = user_model::get_schema();
		if(check_schema){
			return table_exists(user_model::get_table_name(), schema);
		}else{
			return table_exists(user_model::get_table_name());
		}
	}

	virtual bool table_exists(const std::string& name) = 0;
	bool table_exists(const std::string& name, table_schema& schema);
	virtual table_schema get_table_schema(const std::string& name) = 0;
	virtual std::string build_query_string(const query_base& query);

	/**
	 * inserts the provided model or updates it if exists
	 */
//	virtual void save(const model_base& model) = 0;
	//non const model_base ref since auto increment pkey might be altered
	/**
	 * insert a new row for provided model
	 * @param update_auto_inc_pkey whether or not make additional query
	 * for extracting inserted row id and updating provided model
	 */
	virtual void insert(model_base& model, bool update_auto_inc_pkey = false) = 0;
//	virtual void update(const model_base& model) = 0;

	/**
	 * this function assumes query has already been checked against model schema
	 */
	template<class user_model>
	std::vector<user_model> execute(const query<user_model, query_type::_select_many>& query){
		result_table result = execute_flat(query);
		std::vector<user_model> result_models(result.get_num_rows());

		for(size_t i = 0; i < result.get_num_rows(); i++){
			//TODO: no getter here ?
			//TODO: are result columns guaranteed to have same attributes order?
			//todo: optimize the extra copying

			//todo: remove the default ctor of the nullable_field_handle
			//remove the creator if possible
			//add nullable_field from_string fn for parsing database return data
			//add and use get_field_value that takes a nullable_field_handle& to fill with string data
			//so u have to move code in set_field_value to get_field_value
			std::map<std::string, nullable_field_handle>& obj_attribs = result_models[i].attributes;
			for(auto& attrib_element : obj_attribs){
				result.get_field_value(i, attrib_element.first, attrib_element.second);
			}
		}
		return result_models;
	}

	//todo: should return optional<user_model>
	//todo: make sure "limit" is set here
	template<class user_model>
	user_model execute(const query<user_model, query_type::_select_single>& query){
		result_table result = execute_flat(query);
		user_model model;

		if(result.get_num_rows() > 0){
			for(auto& attrib_element : model.attributes){
				result.get_field_value(0, attrib_element.first, attrib_element.second);
			}
		}
		return model;
	}

	virtual result_table execute_flat(const query_base& query) = 0;

	virtual result_table execute_flat(const std::string& query_string) = 0;

	/**
	 * @return # rows affected
	 */
	virtual std::size_t execute_void(const std::string& query_string) = 0;
	virtual ~db_session_base();
protected:
	//TODO: move to model_base OR create class schema instead of typedef and override << for it
	/**
	 * prints comma separated schema map keys ex: "`c1`, `c2` ,..." to specified stream
	 */
	static void print_col_names(std::ostream& stream, const table_schema& schema);

	/**
	 * prints comma separated model attribute values ex: "v1,v2,..." to specified stream<br>
	 * schema is needed for type checking
	 */
	//TODO: commas needed
	static void print_attrib_values(std::ostream& stream, const table_schema& schema, const attribs_map& attribs);
};

} /* namespace ormplusplus */

#endif /* INCLUDE_DB_SESSION_BASE_H_ */
