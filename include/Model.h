#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "typestring.hh"

#include "ModelBase.h"
#include "AttributeInitializer.h"
#include "Query.h"

//macro overloading
#define GET_MACRO(_1,_2,NAME,...) NAME
#define BOUND_MODEL(...) GET_MACRO(__VA_ARGS__, BOUND_MODEL_2, BOUND_MODEL_1)(__VA_ARGS__)

#define BOUND_MODEL_1(CLASS_NAME) class CLASS_NAME : public ORMPlusPlus::Model<CLASS_NAME, typestring_is(#CLASS_NAME)>
#define BOUND_MODEL_2(CLASS_NAME, TABLE_NAME) class CLASS_NAME : public ORMPlusPlus::Model<CLASS_NAME, typestring_is(TABLE_NAME)>
#define DEFINE_ATTRIB(DATATYPE, NAME) DATATYPE NAME = initializeAttrib<DATATYPE>(#NAME)

namespace ORMPlusPlus{

template<class UserModel, class TableName>
class Model : public ModelBase
{
private:
	static std::string tableName;
	static TableSchema schema;

//	template<class AttribType>
//	AttribType* addAttributeVariable(std::string name){
//		NullableFieldBase::deduceDataType<AttribType>();	//called to make sure AttribType is supported
//		attributes[name].reset(new AttribType());
//		return (AttribType*)attributes[name].get();
//	}

public:

	Model<UserModel, TableName>()
	: ModelBase(tableName, schema)
	{
	}

	Model<UserModel, TableName>(const Model<UserModel, TableName>& that) = delete;
	Model<UserModel, TableName>(Model<UserModel, TableName>&& that) = default;

	static void copy(const Model<UserModel, TableName>& src, UserModel& dest){
		dest.attributes = src.attributes;
	}

	UserModel clone(){
		UserModel obj;
		copy(*this, obj);
		return obj;
	};

	static std::vector<UserModel> clone(const std::vector<UserModel>& source){
		std::vector<UserModel> target(source.size());
		for(int i = 0; i < source.size(); i++){
			copy(source[i], target[i]);
		}
		return target;
	}

	Model<UserModel, TableName>& operator=(Model<UserModel, TableName>& that){
		this->attributes = that.attributes;
		return *this;
	}

	static std::vector<UserModel> get(){
		Query<UserModel> query;
		return query.get();
	}

	static std::string getTableName(){
		return tableName;
	}

	static Query<UserModel> where(std::vector<QueryCondition> conditions){
		return Query<UserModel>(conditions);
	}

	static Query<UserModel> where(QueryCondition condition){
		return Query<UserModel>({condition});
	}

	//TODO: add another variant that takes no template parameters and performs the withDefault() check in runtime
	//		this will eliminate the need for macros
	template<typename AttribType>
	AttributeInitializer<AttribType> initializeAttrib(const std::string& name){
		bool columnAlreadyAdded = !addColumnIfNotExists(name, typeid(AttribType).hash_code());
		//TODO: move logic to ModelBase
		NullableFieldBase& field = attributes.emplace(name, NullableFieldBase(AttribType::getPrimitiveType())).first->second;
		if(columnAlreadyAdded){
			return {field, nullptr};
		}else{
			return {field, &schema[name]};
		}
	}

	/**
	 * adds a column to model schema
	 * @return true at success, false if already exists
	 */
	static bool addColumnIfNotExists(const std::string& name, std::size_t typeHash){
		if(columnExists(name)){
			return false;
		}else{
			schema.emplace(name, TableColumn(name, typeHash));
			return true;
		}
	}

	static bool columnExists(std::string name){
		return schema.find(name) != schema.end();
	}

	static TableSchema getDBSchema(){
		return schema;
	}
		
};

template<class UserModel, class TableName>
TableSchema Model<UserModel, TableName>::schema;
template<class UserModel, class TableName>
std::string Model<UserModel, TableName>::tableName = TableName::data();

}

#endif MODEL_H
