#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

#include "NullableFieldBase.h"
#include "AttributeInitializerBase.h"

namespace ORMPlusPlus{

/**
 * Facilitates interaction with the nullable field
 */

template <class PrimitiveType>
class NullableField;

template<class PrimitiveType>
std::ostream& operator<<(std::ostream&, const NullableField<PrimitiveType>&);
template<class PrimitiveType>
std::istream& operator>>(std::istream&, const NullableField<PrimitiveType>&);

/**
 * this class can be used either as a shell for a NullableFieldBase that exists some where else <br>
 * or as a container for NullableFieldBase
 */
template<class PrimitiveType>
class NullableField{
private:
	NullableFieldBase* NFBasePtr = nullptr;
	bool isPtrOwner = false;//should be false if used as shell
	PrimitiveType& valueRef;//useful at debugging
public:
	static const std::type_info& getPrimitiveType()
	{
		return typeid(PrimitiveType);
	}

	NullableField()
	: NFBasePtr(new NullableFieldBase(typeid(PrimitiveType).hash_code())),
	  isPtrOwner(true),
	  valueRef(NFBasePtr->getValueRef<PrimitiveType>())
	{
	}

	NullableField(const NullableField& that)
	: NFBasePtr(new NullableFieldBase(*that.NFBasePtr)),
	  isPtrOwner(true),
	  valueRef(NFBasePtr->getValueRef<PrimitiveType>())
	{
	}

	NullableField(const NullableField&& that)
	: NFBasePtr(new NullableFieldBase(std::move(*that.NFBasePtr))),
	  isPtrOwner(true),
	  valueRef(NFBasePtr->getValueRef<PrimitiveType>())
	{
	}

	NullableField(NullableFieldBase& nullableFieldBase) //todo: convert to NullableFieldBase&&
	: NFBasePtr(&nullableFieldBase),
	  isPtrOwner(false),
	  valueRef(NFBasePtr->getValueRef<PrimitiveType>())
	{
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

	bool operator==(const NullableField& that) const
	{
		if(this->isNull() && that.isNull()){
			return true;
		}else if(this->isNull() || that.isNull()){
			return false;
		}else{
			return this->getValueRef() == that.getValueRef();
		}
	}

	bool operator==(const PrimitiveType& value) const
	{
		if(this->isNull()){
			return false;
		}else{
			return this->getValueRef() == value;
		}
	}

	bool operator!=(const NullableField& that) const
	{
		return !operator==(that.getValueRef());
	}

	bool operator!=(const PrimitiveType& value) const
	{
		return !operator==(getValueRef() == value);
	}

	bool operator>(const NullableField& that) const
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() > that.getValueRef();
	}

	bool operator>(const PrimitiveType& value) const
	{
		assertLHSNotNull(*this);
		return this->getValueRef() > value;
	}

	bool operator>=(const NullableField& that) const
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() >= that.getValueRef();
	}

	bool operator>=(const PrimitiveType& value) const
	{
		assertLHSNotNull(*this);
		return this->getValueRef() >= value;
	}

	bool operator<(const NullableField& that) const
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() < that.getValueRef();
	}

	bool operator<(const PrimitiveType& value) const
	{
		assertLHSNotNull(*this);
		return this->getValueRef() < value;
	}

	bool operator<=(const NullableField& that) const
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->getValueRef() <= that.getValueRef();
	}

	bool operator<=(const PrimitiveType& value) const
	{
		assertLHSNotNull(*this);
		return this->getValueRef() <= value;
	}

	operator PrimitiveType()
	{
		return getValueRef();
	}

	std::string toString() const
	{
		return NFBasePtr->toString();
	}

	~NullableField()
	{
		if(isPtrOwner && NFBasePtr != nullptr){
			delete NFBasePtr;
		}
	}

	PrimitiveType& getValueRef() const
	{
		return NFBasePtr->getValueRef<PrimitiveType>();
	}

	NullableFieldBase& getBaseRef()
	{
		return *NFBasePtr;
	}

	bool isNull() const
	{
		return NFBasePtr->isNull();
	}

	friend std::ostream& operator<< <>(std::ostream&, const NullableField&);
	friend std::istream& operator>> <>(std::istream&, const NullableField&);

};

template<class PrimitiveType>
std::ostream& operator<<(std::ostream& os, const NullableField<PrimitiveType>& field)
{
	os << field.getValueRef();
    return os;
}

template<class PrimitiveType>
std::istream& operator>>(std::istream& is, const NullableField<PrimitiveType>& field)
{
	is >> field.getValueRef();
    return is;
}

//remember to add the necessary item to typeInfoMap at adding new specializations
typedef NullableField<int> Integer;
typedef NullableField<long> Long;
typedef NullableField<float> Float;
typedef NullableField<double> Double;
typedef NullableField<std::string> String;
typedef NullableField<::tm> DateTime;
//TODO: is this specialization essential ?
typedef NullableField<nullptr_t> Null;


//TODO: replace by an empty constructor in NullableField
//to be used like if x == Null() -maybe?-
//extern const Null NullValue(nullptr);

}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
