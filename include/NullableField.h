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
template<class UserModel, class TableName, class AttribType>
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

	PrimitiveType& get(){
		return primitiveValue;
	}

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
