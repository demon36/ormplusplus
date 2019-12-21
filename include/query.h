#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#include "db.h"
#include "query_base.h"

namespace ormplusplus{

/**
 * this class is responsible for storing query parameters and checking their
 * validity against model schema, and returning instance(s) of UserModel upon request
 */
template<class user_model>
class query : public query_base{
public:
	query() = delete;// : QueryBase(UserModel::getTableName()) {}
	query(const query& _query) : query_base(_query) {}
	query(const std::vector<query_condition>& _conditions) : query_base(user_model::getTableName(), user_model::getSchema()){
		//TODO: assert conditions comply to column names and types
		conditions = _conditions;
	}

	std::vector<user_model> select(){
		//TODO: assert schema is not empty for type
		type = query_type::_select;
		return db::get_default_session().execute<user_model>(*this);
	}

	user_model select_one(){
		this->limit = 1;
		std::vector<user_model> objects = select();
		if(objects.empty()){
			throw std::runtime_error("Calling selectOne() on a query that returned zero elements");
		}else{
			return objects[0].clone();
		}
	}

//	operator std::vector<UserModel>(){
//		std::vector<UserModel> modelsList;
//		return modelsList;
//	}

	//TODO: throw exception if a decorator got called twice
//	void remove();
//	Query& update(std::vector<std::string>);
//	void set(std::vector<std::string>);
//	long count();
//	long sum();
};

}

#endif /* INCLUDE_QUERY_H_ */
