#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>

#include "ModelBase.h"
#include "Query.h"

using namespace std;

#define DEFINE_ATTR(DATATYPE, NAME) DATATYPE& NAME = mapToField<DATATYPE>(#NAME)
#define COLUMN(NAME)(#NAME)

namespace ORMPlusPlus{

template<class UserModel>
class Model : public ModelBase
{
public:
	virtual string getTableName() = 0;
	virtual ~Model(){};

	static std::vector<UserModel> get(){
		Query<UserModel> query;
		return query.get();
	}

	static Query<UserModel> where(std::vector<QueryCondition> conditions){
		Query<UserModel> query;
		return query;
	}

	template<typename FieldType>
	FieldType& mapToField(string attributeName){
		fields[attributeName] = new FieldType(this, attributeName);
		return static_cast<FieldType&>(*fields[attributeName]);
	}

};
}

#endif MODEL_H
