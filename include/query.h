#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#include "query_base.h"

namespace ormplusplus{

/**
 * this class is responsible for storing query parameters and checking their
 * validity against model schema, and returning instance(s) of user_model upon request
 */
template<class user_model, query_type _type>
class query : public query_base{
public:
	//todo: remove this ctor if useless
	query() : query_base(user_model::get_table_name(), user_model::get_schema(), _type) {
	}

	query(const query& _query) : query_base(_query) {}

	query& where(const std::vector<query_condition>& _conditions){
		for(const query_condition& condition : _conditions){
			conditions.push_back(condition);
		}
		return *this;
	}

	query& where(const query_condition& condition){
		conditions.push_back(condition);
		return *this;
	}

	/*
	user_model select_one(){
		this->limit = 1;
		std::vector<user_model> objects = select();
		if(objects.empty()){
			throw std::runtime_error("Calling select_one() on a query that returned zero elements");
		}else{
			return objects[0].clone();
		}
	}*/

//	operator std::vector<user_model>(){
//		std::vector<user_model> models_list;
//		return models_list;
//	}

	//TODO: throw exception if a decorator got called twice

//	void remove();
//TODO: assert conditions comply to column names and types
//	Query& where()
//	Query& update(std::vector<std::string>);
//	void set(std::vector<std::string>);
//	long count();
//	long sum();
};

}

#endif /* INCLUDE_QUERY_H_ */
