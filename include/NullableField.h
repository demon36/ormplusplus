#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

#include <string>
#include <sstream>
#include <iostream>

#include "ModelBase.h"
#include "AttributeInitializer.h"

using namespace std;

namespace ORMPlusPlus{

class ModelBase;

enum DataType{
	_Integer,
	_String,
};

class NullableFieldBase{
public:
	virtual string toString(){ return ""; };
};

template<class DerivedType, class PrimitiveType>
class NullableField : public NullableFieldBase{
	ModelBase* modelPtr = nullptr;
	string columnName = "";
	bool isNull = true;
protected:
	PrimitiveType primitiveValue;
	bool requireQuotes = true;
public:

	NullableField();

	//attach to model
	NullableField(ModelBase* OwnerModel, string attributeName){
		this->columnName = attributeName;
		modelPtr = OwnerModel;
	}

	NullableField(PrimitiveType value){
		primitiveValue = value;
		isNull = false;
	}

	template<class UserModel>
	NullableField(AttributeInitializer<UserModel, DerivedType> initr){
		std::cout<<"NullableField(AttributeInitializer<UserModel, DerivedType> initr)\n";
		columnName = "initr";
	}

	DerivedType& withColumnName(string customColumnName){
		if(modelPtr == nullptr){
			throw runtime_error("NullableField is not bound to model");
		}
		columnName = customColumnName;
		return static_cast<DerivedType&>(*this);
	}

	string getColumnName(){
		return columnName;
	}

	DerivedType& asPrimaryKey(){
		modelPtr->columns[columnName].setAsPrimaryKey();
		return static_cast<DerivedType&>(*this);
	}

	DerivedType& setToNull(){
		isNull = true;
		return static_cast<DerivedType&>(*this);
	}

	PrimitiveType& get(){
		return primitiveValue;
	}

	DerivedType& withDefault(PrimitiveType value){
		primitiveValue = value;
		return static_cast<DerivedType&>(*this);
	}

	string toString(){
		stringstream ss;
		ss << primitiveValue;
		return ss.str();
	}
};

class Integer : public NullableField<Integer, int>{
	using NullableField::NullableField;
public:
	static DataType getTypeName(){
		return DataType::_Integer;
	}
};

class String : public NullableField<String, string>{
	using NullableField::NullableField;
public:
	static DataType getTypeName(){
		return DataType::_String;
	}
};

const NullableFieldBase NullValue;
}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
