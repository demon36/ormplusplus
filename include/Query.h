#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#include <string>
#include <sstream>

#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>

#include "QueryCondition.h"
#include "DB.h"

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace std;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;


namespace ORMPlusPlus{

template<class UserModel>
class Query{
	stringstream whereClause;
public:
	Query(){}
	Query(std::vector<QueryCondition> conditions){
		whereClause << " WHERE ";
		for(int i = 0; i < conditions.size(); i++){
			whereClause << conditions[i].getColumnName() << " "
					<< conditions[i].getOperator() << " "
					<< conditions[i].getValue()->toString() << " ";
			if(i < conditions.size() - 1){
				whereClause << " AND ";
			}else{
				whereClause << ";";
			}
		}
	}

	std::vector<UserModel> get(){
		stringstream queryStream;
		//todo: select specific columns before additional error check
		queryStream << "SELECT * FROM " << UserModel::getTableName() << whereClause.str();
		Statement select(*DB::session);

		std::vector<UserModel> modelsList;
		select << queryStream.str();
		select.execute();
		RecordSet rs(select);
		int x;
		rs[""].convert<int>(x);
		while (!select.done())
		{
			select.execute();
		}

	}

	void remove();
	Query& update(std::vector<string>);

	void set(std::vector<string>);
	long long count();
	long long sum();
};

}



#endif /* INCLUDE_QUERY_H_ */
