#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "ModelBase.h"
#include "AttributeInitializer.h"
#include "Query.h"

using namespace std;

#define BOUND_MODEL(CLASS_NAME) class CLASS_NAME : public ORMPlusPlus::Model<CLASS_NAME>
#define DEFINE_ATTR(DATATYPE, NAME) DATATYPE& NAME = mapToField<DATATYPE>(#NAME)
#define COLUMN(NAME)(#NAME)

namespace ORMPlusPlus{

template<class UserModel>
class Model : public ModelBase
{
private:
//	static AttributeInitializer<UserModel> attribInitializer;
	template<class NullableType>
	friend class AttributeInitializer<UserModel, NullableType>;
public:
	static std::map<std::string, TableColumn> columns;
	Model<UserModel>(){};
	Model<UserModel>(Model<UserModel>& model) = delete;
	static std::vector<UserModel> get(){
		Query<UserModel> query;
		return query.get();
	}

	static Query<UserModel> where(std::vector<QueryCondition> conditions){
		Query<UserModel> query;
		return query;
	}

//	template<class NullableType>
//	static AttributeInitializer<UserModel>& initialize(std::string columnName){
//		return attribInitializer;
//	}

	template<class NullableType>
	static shared_ptr<AttributeInitializer<UserModel, NullableType>> initialize2(UserModel* modelInstance){
		return make_shared<AttributeInitializer<UserModel, NullableType>>(modelInstance);
	}

};

template<class UserModel>
std::map<std::string, TableColumn> Model<UserModel>::columns;
}

#endif MODEL_H
