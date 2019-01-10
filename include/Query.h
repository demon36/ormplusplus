#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#include "QueryBase.h"
#include "DB.h"

namespace ORMPlusPlus{

/**
 * this class is responsible for storing query parameters and checking their
 * validity against model schema, and returning instance(s) of UserModel upon request
 */
template<class UserModel>
class Query : public QueryBase{
public:
	Query() = delete;// : QueryBase(UserModel::getTableName()) {}
	Query(const Query& query) : QueryBase(query) {}
	Query(const std::vector<QueryCondition>& _conditions) : QueryBase(UserModel::getTableName(), UserModel::getSchema()){
		//TODO: assert conditions comply to column names and types
		conditions = _conditions;
	}

	std::vector<UserModel> select(){
		//TODO: assert schema is not empty for type
		type = QueryType::_Select;
		return DB::getDefaultSession().execute<UserModel>(*this);
//		std::vector<UserModel> modelsList;
//		UserModel test();
//		Poco::Data::RecordSet rs = select();
//		int x;
//		rs[""].convert<int>(x);
//		return modelsList;
	}

//	operator std::vector<UserModel>(){
//		std::vector<UserModel> modelsList;
//		return modelsList;
//	}

//	void remove();
//	Query& update(std::vector<std::string>);
//	void set(std::vector<std::string>);
//	long count();
//	long sum();
};

}



#endif /* INCLUDE_QUERY_H_ */
