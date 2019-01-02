#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

//TODO: if gcc only
#include <cxxabi.h>
#define quote(x) #x

#include "NullableFieldBase.h"

namespace ORMPlusPlus{

class AttributeInitializerBase;

/**
 * this class can be used either as a shell for a NullableFieldBase that exists some where else <br>
 * or as a container for NullableFieldBase
 */
template<class PrimitiveType>
class NullableField{
private:
	NullableFieldBase* NFBasePtr = nullptr;
	bool isPtrOwner = false;//should be false if used as shell
public:
	static const std::type_info& getPrimitiveType(){
		return typeid(PrimitiveType);
	}

	static std::string getDemangledName(){
		//TODO: error check
		int status;
		char* demangled = abi::__cxa_demangle(typeid(PrimitiveType).name(),0,0,&status);
		std::string demangledStr(demangled);
		free(demangled);
		return demangledStr;
	}

	NullableField() : isPtrOwner(true)
	{
		NFBasePtr = new NullableFieldBase(typeid(PrimitiveType));
	}

	NullableField(const NullableField<PrimitiveType>& that) : isPtrOwner(true)
	{
		NFBasePtr = new NullableFieldBase(*that.NFBasePtr);
	}

	NullableField(const NullableField<PrimitiveType>&& that) : isPtrOwner(true)
	{
		NFBasePtr = new NullableFieldBase(std::move(*that.NFBasePtr));
	}

	NullableField(NullableFieldBase& nullableFieldBase) : isPtrOwner(false)
	{
		NFBasePtr = &nullableFieldBase;
	}

	NullableField(const AttributeInitializerBase& attribInitializer)
	: NullableField(attribInitializer.getNullableFieldBaseRef())
	{

	}

	NullableField(const PrimitiveType& value) : NullableField()
	{
		NFBasePtr->setValue<PrimitiveType>(value);
	}

	NullableField& operator=(const NullableField& that)
	{
		NFBasePtr->setValue<PrimitiveType>(that.getValueRef());
		return *this;
	}

	NullableField& operator=(const PrimitiveType& value)
	{
		NFBasePtr->setValue<PrimitiveType>(value);
		return *this;
	}

	bool operator==(const NullableField& that)
	{
		if(this->isNull() && that.isNull()){
			return true;
		}else if(this->isNull() || that.isNull()){
			return false;
		}else{
			return this->getValueRef() == that.getValueRef();
		}
	}

	bool operator==(const PrimitiveType& value)
	{
		if(this->isNull()){
			return false;
		}else{
			return this->getValueRef() == value;
		}
	}

	bool operator!=(const NullableField& that)
	{
		return !operator==(that.getValueRef());
	}

	bool operator!=(const PrimitiveType& value)
	{
		return !operator==(getValueRef() == value);
	}

	bool operator>(const NullableField& that)
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() > that.getValueRef();
	}

	bool operator>(const PrimitiveType& value)
	{
		assertLHSNotNull(*this);
		return this->getValueRef() > value;
	}

	bool operator>=(const NullableField& that)
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() >= that.getValueRef();
	}

	bool operator>=(const PrimitiveType& value)
	{
		assertLHSNotNull(*this);
		return this->getValueRef() >= value;
	}

	bool operator<(const NullableField& that)
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() < that.getValueRef();
	}

	bool operator<(const PrimitiveType& value)
	{
		assertLHSNotNull(*this);
		return this->getValueRef() < value;
	}

	bool operator<=(const NullableField& that)
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() <= that.getValueRef();
	}

	bool operator<=(const PrimitiveType& value)
	{
		assertLHSNotNull(*this);
		return this->getValueRef() <= value;
	}

	operator PrimitiveType&(){
		return getValueRef();
	}

	operator PrimitiveType(){
		return getValueRef();
	}

	std::string toString(){
		//TODO: move this to NullableFieldBase
		std::stringstream ss;
		ss << getValueRef();
		return ss.str();
	}

	~NullableField<PrimitiveType>(){
		if(isPtrOwner && NFBasePtr != nullptr){
			delete NFBasePtr;
		}
	}

	const PrimitiveType& getValueRef() const{
		return NFBasePtr->getValueRef<PrimitiveType>();
	}

	NullableFieldBase& getBaseRef(){
		return *NFBasePtr;
	}

	bool isNull(){
		return NFBasePtr->isNull();
	}

};

//TODO: replace by an empty constructor in NullableField
//to be used like if x == Null() -maybe?-
//extern const Null NullValue(nullptr);

}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
