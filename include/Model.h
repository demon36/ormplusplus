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
#define DEFINE_ATTRIB(DATATYPE, NAME) DATATYPE& NAME = initializeAttrib<DATATYPE>(#NAME)

namespace ORMPlusPlus{

template<class UserModel>
class Model
{
private:
	static std::map<std::string, TableColumn> columnDefs;
	std::map<std::string, unique_ptr<NullableFieldBase>> attributes;

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
			columnDefs.insert({name, TableColumn(columnType, name)});
		}
	}

	template<class AttribType>
	AttribType* addAttributeVariable(std::string name){
		deduceDataType<AttribType>();	//called to make sure AttribType is supported
		attributes[name].reset(new AttribType());
		return (AttribType*)attributes[name].get();
	}

public:

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
				
	template<typename AttribType>
	AttributeInitializer<UserModel, AttribType> initializeAttrib(std::string name){
		addColumnIfNotExists<AttribType>(name);
		AttribType* attribVariablePtr = addAttributeVariable<AttribType>(name);
		AttributeInitializer<UserModel, AttribType> initr(attribVariablePtr, name);
		return initr;
	}

	static bool columnExists(std::string name){
		return columnDefs.find(name) != columnDefs.end();
	}
		
};

template<class UserModel>
std::map<std::string, TableColumn> Model<UserModel>::columnDefs;
}

#endif MODEL_H
