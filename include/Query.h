#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#include <string>

#include "QueryCondition.h"

using namespace std;

namespace ORMPlusPlus{

template<class UserModel>
class Query{
	string whereClause = "WHERE ";
public:
	Query(){};
	Query(vector<QueryCondition> conditions){
		for(auto& condition : conditions){
		}
	}

	std::vector<UserModel> get(){

	}

	void remove();
	Query& update(std::vector<string>);
	void set(std::vector<string>);
	long long count();
	long long sum();
};

}



#endif /* INCLUDE_QUERY_H_ */
