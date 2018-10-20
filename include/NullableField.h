#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

#include <string>
#include <sstream>

#include "ModelBase.h"

using namespace std;

namespace ORMPlusPlus{

class ModelBase;

class NullableFieldBase{
public:
	virtual string toString(){ return ""; };
};

template<class DerivedType, class PrimitiveType>
class NullableField : public NullableFieldBase{
	ModelBase* modelPtr = nullptr;
	//column name in the database
	string columnName = "";
	bool isNull = true;
	bool isPrimaryKey = false;
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
		isPrimaryKey = true;
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
};

class String : public NullableField<String, string>{
	using NullableField::NullableField;
};

const NullableFieldBase NullValue;
}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
