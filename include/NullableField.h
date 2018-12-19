#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

#include <string>
#include <sstream>
#include <iostream>
#include <memory>

#include "ModelBase.h"
#include "AttributeInitializer.h"

namespace ORMPlusPlus{

class ModelBase;
template<class UserModel, const char* TableName, class AttribType>
class AttributeInitializer;

class NullableFieldBase{
public:
	virtual std::string toString(){ return ""; };
};

template<class PrimitiveType>
class NullableField : public NullableFieldBase{
	bool isNull = true;
protected:
	PrimitiveType primitiveValue;
	bool requireQuotes = true;
public:

	NullableField(){}

	NullableField(PrimitiveType value){
		primitiveValue = value;
		isNull = false;
	}

	NullableField& operator=(const NullableField& that) // copy assignment
	{
		this->primitiveValue = that.primitiveValue;
		return *this;
	}

	//attach to model
//		template<class UserModel>
//	NullableField(Model<UserModel>* OwnerModel, string attributeName){
//		this->columnName = attributeName;
//		modelPtr = OwnerModel;
//	}

//	DerivedType& withColumnName(string customColumnName){
//		if(modelPtr == nullptr){
//			throw runtime_error("NullableField is not bound to model");
//		}
//		columnName = customColumnName;
//		return static_cast<DerivedType&>(*this);
//	}

//	DerivedType& asPrimaryKey(){
////		modelPtr->columns[columnName].setAsPrimaryKey();
//		return static_cast<DerivedType&>(*this);
//	}

//	DerivedType& setToNull(){
//		isNull = true;
//		return static_cast<DerivedType&>(*this);
//	}

	PrimitiveType& get(){
		return primitiveValue;
	}

//	DerivedType& withDefault(PrimitiveType value){
//		primitiveValue = value;
//		return static_cast<DerivedType&>(*this);
//	}

	std::string toString(){
		std::stringstream ss;
		ss << primitiveValue;
		return ss.str();
	}
};

typedef NullableField<std::string> String;
typedef NullableField<int> Integer;

//class String : public NullableField<string>{
//	using NullableField::NullableField;
//public:
//	static DataType getTypeName(){
//		return DataType::_String;
//	}
//};

//class Integer : public NullableField<int>{
//	using NullableField::NullableField;
//public:
//	static DataType getTypeName(){
//		return DataType::_Integer;
//	}
//};

const NullableFieldBase NullValue;
}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
