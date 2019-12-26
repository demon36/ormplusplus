#ifndef INCLUDE_DB_SESSION_BASE_H_
#define INCLUDE_DB_SESSION_BASE_H_

#include "table_column.h"
#include "query_base.h"
#include "model_base.h"
#include "result_table.h"

namespace ormplusplus {

class db_session_base {
public:
	db_session_base();
	virtual void create_table(const std::string& name, const table_schema& schema) = 0;
	void drop_table(const std::string& name);
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
	std::vector<user_model> execute(const query_base& query){
		std::vector<user_model> results;

		result_table result = execute_flat(query);
		for(size_t i = 0; i < result.get_num_rows(); i++){
			user_model obj;
			//TODO: no getter here ?
			//TODO: are result columns guaranteed to have same attributes order?
			//todo: optimize the extra copying

			//todo: remove the default ctor of the nullable_field_handle
			//remove the creator if possible
			//add nullable_field from_string fn for parsing database return data
			//add and use get_field_value that takes a nullable_field_handle& to fill with string data
			//so u have to move code in set_field_value to get_field_value
			std::map<std::string, nullable_field_handle>& obj_attribs = obj.attributes;
			for(auto& attrib_element : obj_attribs){
				result.get_field_value(i, attrib_element.first, attrib_element.second);
			}
			results.emplace_back(obj.clone());
		}
		return results;
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
