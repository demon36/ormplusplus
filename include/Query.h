#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#include "QueryBase.h"

namespace ORMPlusPlus{

template<class UserModel>
class Query : public QueryBase{

public:
	Query() : QueryBase(UserModel::getTableName()) {}
	Query(const Query& query) : QueryBase(query) {}
	Query(std::vector<QueryCondition>& conditions) : QueryBase(UserModel::getTableName()){
		where(conditions);
	}

	std::vector<UserModel> get(){
		std::vector<UserModel> modelsList;
		UserModel test();
//		Poco::Data::RecordSet rs = select();
//		int x;
//		rs[""].convert<int>(x);
		return modelsList;
	}

//	operator std::vector<UserModel>(){
//		std::vector<UserModel> modelsList;
//		return modelsList;
//	}

	void remove();
	Query& update(std::vector<std::string>);
	void set(std::vector<std::string>);
	long long count();
	long long sum();
};

}



#endif /* INCLUDE_QUERY_H_ */
