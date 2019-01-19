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

//TODO: get rid of model binding macros
//macro overloading
#define GET_MACRO(_1,_2,NAME,...) NAME
#define BOUND_MODEL(...) GET_MACRO(__VA_ARGS__, BOUND_MODEL_2, BOUND_MODEL_1)(__VA_ARGS__)

#define BOUND_MODEL_1(CLASS_NAME) class CLASS_NAME : public ORMPlusPlus::Model<CLASS_NAME, typestring_is(#CLASS_NAME)>
#define BOUND_MODEL_2(CLASS_NAME, TABLE_NAME) class CLASS_NAME : public ORMPlusPlus::Model<CLASS_NAME, typestring_is(TABLE_NAME)>

//creates a set of template parameters from a string literal
#define _ typestring_is
//TODO: facilitate writing conditions using sth like:
//#define DEFINE_ATTRIB(DATATYPE, NAME) static std::string _ ## NAME(){static std::string _s = #NAME; return _s;}; DATATYPE NAME = initializeAttrib<DATATYPE>(#NAME)
//or a static reference that is initialized to a schema entry
//this is better static const TableColumn& ID_(){ return schema[""]; };
//so that queries can like where({MyModel::_ID(), "=", 3})
#define DEFINE_ATTRIB(DATATYPE, NAME) DATATYPE NAME = initializeAttrib<DATATYPE>(#NAME)

namespace ORMPlusPlus{

template<class UserModel, class TableName = typestring_is("")>
class Model : public ModelBase
{
private:
protected:
	static std::string tableName;
	static TableSchema schema;
	//set to true after first instance get created
	static bool schemaBuilt;

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
public:

	Model<UserModel, TableName>()
	: ModelBase(tableName, schema)
	{
		schemaBuilt = true;
	}

	Model<UserModel, TableName>(const Model<UserModel, TableName>& that) = delete;
	Model<UserModel, TableName>(Model<UserModel, TableName>&& that) = default;

	static void copy(const Model<UserModel, TableName>& src, UserModel& dest){
		dest.setAttributes(src.attributes);
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

	//TODO: implement nested conditions
	static Query<UserModel> where(std::vector<QueryCondition> conditions){
		return Query<UserModel>(conditions);
	}

	static Query<UserModel> where(QueryCondition condition){
		return Query<UserModel>({condition});
	}

	//TODO: add another variant that takes no template parameters and performs the withDefault() check in runtime
	//		this will eliminate the need for macros
	//TODO: should this fn be renamed to bindAttribute ?
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

	static bool columnExists(std::string name){
		return getSchema().find(name) != getSchema().end();
	}

	static const TableSchema& getSchema(){
		//built schema by creating a model instance before processing the query
		if(!schemaBuilt){
			UserModel dummyInstance;
		}
		return schema;
	}
		
};

template<class UserModel, class TableName>
TableSchema Model<UserModel, TableName>::schema;

template<class UserModel, class TableName>
std::string Model<UserModel, TableName>::tableName =
		std::string(TableName::data()).empty() ?
		ModelBase::getTypeName(typeid(UserModel)) : TableName::data();
template<class UserModel, class TableName>
bool Model<UserModel, TableName>::schemaBuilt = false;

}



#endif MODEL_H
