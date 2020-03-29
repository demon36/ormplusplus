#ifndef INCLUDE_MODEL_BASE_H_
#define INCLUDE_MODEL_BASE_H_

#include <vector>
#include <map>
#include <memory>

#include "nullable_field_handle.h"
#include "table_column.h"

namespace ormplusplus{

typedef std::map<std::string, nullable_field_handle> attribs_map;

class model_base{
	friend class db_session_base;
private:
	const std::string& table_name_ref;
	table_schema& schema_ref;//owner is always model<T, U>
protected:
	attribs_map attributes;
public:

	//TODO: would enable_if help here ?
	template <class T>
	static bool is_user_model_class(){
		return std::is_base_of<model_base, T>::value;
	}

	template <class T>
	static void assert_class_is_user_model(){
		if(!is_user_model_class<T>()){
			throw std::runtime_error("template parameter is not a legitimate child of model_base");
		}
	}

	model_base(const std::string& table_name, table_schema& schema);
	model_base(const model_base& that);
	model_base(model_base&& that);
	table_column& get_col_ref(const std::string& name) const;
	std::string get_table_name() const;
	const table_schema& get_schema() const;
	void set_attribs(const attribs_map& values);
	attribs_map& get_attribs();//todo: make reference const
	/**
	 * @return empty string if no primary key columns with auto increment values
	 */
	bool auto_inc_pkey_col_exists() const;
	void set_auto_inc_pkey(long value);
	bool equals(const model_base& that) const;
	/**
	 * inserts a new row or updates existing
	 */
//	void save();
//	void load();
	/**
	 * insert a new row for this model
	 * @param update_auto_inc_pkey whether or not make additional query
	 * for extracting inserted row id and updating current model
	 */
//	void insert(bool update_auto_inc_pkey = false);
//	void update();
//	static std::vector<table_column> get_columns();
//	static void add_column(std::string name, type_info type);
	//TODO: implement
	std::string to_string();
};
}

#endif /* INCLUDE_MODEL_BASE_H_ */
