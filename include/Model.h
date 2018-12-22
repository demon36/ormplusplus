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
#define DEFINE_ATTRIB(DATATYPE, NAME) DATATYPE& NAME = initializeAttrib<DATATYPE>(#NAME)

namespace ORMPlusPlus{

template<class UserModel, class TableName>
class Model : public ModelBase
{
private:
	static std::string tableName;
	static TableSchema schema;

	template<class AttribType>
	static DataType deduceDataType(){
		if (std::is_same<AttribType, String>::value) {
			return DataType::_String;
		}else if(std::is_same<AttribType, Integer>::value){
			return DataType::_Integer;
		}else{
			throw std::runtime_error("Attribute type not supported");
		}
	}

	template<class AttribType>
	static void addColumnIfNotExists(std::string name){
		DataType columnType = deduceDataType<AttribType>();
		if(!columnExists(name)){
			schema.insert({name, TableColumn(name, columnType)});
		}
	}

	template<class AttribType>
	AttribType* addAttributeVariable(std::string name){
		deduceDataType<AttribType>();	//called to make sure AttribType is supported
		attributes[name].reset(new AttribType());
		return (AttribType*)attributes[name].get();
	}

public:

	Model<UserModel, TableName>() : ModelBase(schema, tableName){};
	Model<UserModel, TableName>(Model<UserModel, TableName>& model) = delete;
	static std::vector<UserModel> get(){
		Query<UserModel> query;
		return query.get();
	}

	static std::string getTableName(){
		return tableName;
	}

	static Query<UserModel> where(std::vector<QueryCondition> conditions){
		Query<UserModel> query;
		return query;
	}
				
	template<typename AttribType>
	AttributeInitializer<AttribType> initializeAttrib(std::string name){
		addColumnIfNotExists<AttribType>(name);
		AttribType* attribVariablePtr = addAttributeVariable<AttribType>(name);
		AttributeInitializer<AttribType> initr(attribVariablePtr, name);
		return initr;
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
