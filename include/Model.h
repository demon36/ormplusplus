#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "ModelBase.h"
#include "AttributeInitializer.h"
#include "Query.h"

#define BOUND_MODEL(CLASS_NAME) class CLASS_NAME : public ORMPlusPlus::Model<CLASS_NAME>
#define DEFINE_ATTR(DATATYPE, NAME) DATATYPE& NAME = mapToField<DATATYPE>(#NAME)
#define COLUMN(NAME)(#NAME)

namespace ORMPlusPlus{

template<class UserModel>
class Model
{
private:
//	static AttributeInitializer<UserModel> attribInitializer;
	friend class AttributeInitializer<UserModel>;
	
        std::map<std::string, NullableFieldBase*> attributes;
public:
static std::map<std::string, TableColumn> columnDefs;
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

	template<typename FieldType>
		FieldType& mapToField(std::string attributeName){
		attributes[attributeName] = new FieldType(this, attributeName);
		return static_cast<FieldType&>(*attributes[attributeName]);
	}
                
//	template<class NullableType>
//	static AttributeInitializer<UserModel>& initialize(std::string columnName){
//		return attribInitializer;
//	}

	static shared_ptr<AttributeInitializer<UserModel>> initialize2(UserModel* modelInstance){
		return make_shared<AttributeInitializer<UserModel>>(modelInstance);
	}
        
	shared_ptr<AttributeInitializer<UserModel>> initializeAttrib(){
		shared_ptr<AttributeInitializer<UserModel>> attribInitializerPtr = make_shared<AttributeInitializer<UserModel>>(this);
		return attribInitializerPtr;
	}
        
};

template<class UserModel>
std::map<std::string, TableColumn> Model<UserModel>::columnDefs;
}

#endif MODEL_H
