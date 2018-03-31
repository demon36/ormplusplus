#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>

#include "ORMPlusPlus.h"

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

#define DEFINE_MEMBER(DATATYPE, NAME) DATATYPE NAME = mapToField<DATATYPE>(#NAME)

namespace ORMPlusPlus{

template<class DerivedType, class PrimitiveType>
class NullableField{
	bool isNull = true;
	bool isPrimaryKey = false;
protected:
	void* valuePtr = nullptr;
	bool requireQuotes = true;
public:

	NullableField(void* binding){
		binding = new PrimitiveType;
		valuePtr = binding;
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
		return *static_cast<PrimitiveType*>(valuePtr);
	}

	virtual DerivedType& withDefault(PrimitiveType value) = 0;
};

class Integer : public NullableField<Integer, int>{
public:
	using NullableField::NullableField;

	Integer& withDefault(int value){
		*((int*)valuePtr) = value;
		return *this;
	}
};

template<class Derived>
class Model{
	map<string, void*> fields;
public:
	virtual string getTableName() = 0;

	static std::vector<Derived> findMany(){
//			vector<Derived> list;
//			Derived d;
//			list.push_back(d);
//			return list;
	}

	template<typename T>
	T mapToField(string colName){
		fields[colName] = nullptr;
		T member(fields[colName]);
		return member;
	}

};
}

#endif MODEL_H
