#ifndef INCLUDE_DBH_
#define INCLUDE_DBH_

#include <string>
#include <memory>
#include <type_traits>

#include "table_column.h"
#include "db_session_base.h"

namespace ormplusplus{

template<class user_model, class table_name>
class model;
class model_base;

class db{
private:
	//to overcome adding a separate source file
	static std::shared_ptr<db_session_base>& get_default_session_ptr(){
		static std::shared_ptr<db_session_base> default_db_session;
		return default_db_session;
	}

public:
	//make sure template parameter is a child of Model<UserModel>
	//TODO: check at compile time
	template <class T>
	static bool is_user_model_class(){
		return std::is_base_of<model_base, T>::value;
	}

	template <class T>
	static void assert_class_is_user_model(){
		if(!is_user_model_class<T>()){
			throw std::runtime_error("template parameter is not a legitimate child of ModelBase");
		}
	}

	static void set_default_session(std::shared_ptr<db_session_base> DBSession){
		get_default_session_ptr() = DBSession;
	}

	static db_session_base& get_default_session(){
		if(get_default_session_ptr()){
			return *get_default_session_ptr();
		}else{
			throw std::runtime_error("no default database session is set");
		}
	}

	template <class user_model>
	static void create_or_replace_table(){
		throw std::runtime_error("unimplemented");
	}

	// CREATE TABLE 'client_info' ('age' INT, 'height' INT, 'name' VARCHAR(1024));
	template <class user_model>
	static void create_table(){
		assert_class_is_user_model<user_model>();
		get_default_session().create_table(user_model::getTableName(), user_model::getSchema());
	}

	template <class user_model>
	static void drop_table(){
		get_default_session().drop_table(user_model::getTableName());
	}

	template <class user_model>
	static bool table_exists(bool check_schema = false){
		assert_class_is_user_model<user_model>();
		table_schema schema = user_model::getSchema();
		if(check_schema){
			return get_default_session().table_exists(user_model::getTableName(), schema);
		}else{
			return get_default_session().table_exists(user_model::getTableName());
		}
	}

};

}



#endif /* INCLUDE_DBH_ */
