#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

#include <string>
#include <sstream>

namespace ORMPlusPlus{

class NullableFieldBase{
protected:
	bool hasValue = false;

	static void assertLHSNotNull(const NullableFieldBase& lhs){
		if(lhs.isNull()){
			throw std::runtime_error("comparing with null left hand operand");
 		}
	}

	static void assertRHSNotNull(const NullableFieldBase& rhs){
		if(rhs.isNull()){
			throw std::runtime_error("comparing with null left hand operand");
 		}
	}

public:
	virtual std::string toString(){ return ""; };
	bool isNull() const {
		return !hasValue;
	}
};

template<class PrimitiveType>
class NullableField : public NullableFieldBase{
private:
	PrimitiveType primitiveValue;
	bool requireQuotes = true;

public:

	NullableField(){}

	NullableField(PrimitiveType value){
		setValue(value);
	}

	NullableField& operator=(const NullableField& that) // copy assignment
	{
		setValue(that.primitiveValue);
		return *this;
	}

	NullableField& operator=(const PrimitiveType& value) // copy assignment
	{
		setValue(value);
		return *this;
	}

	bool operator==(const NullableField& that) // copy assignment
	{
		if(this->isNull() && that.isNull()){
			return true;
		}else if(this->isNull() || that.isNull()){
			return false;
		}else{
			return this->primitiveValue == that.primitiveValue;
		}
	}

	bool operator==(const PrimitiveType& value) // copy assignment
	{
		if(this->isNull()){
			return false;
		}else{
			return this->primitiveValue == value;
		}
	}

	bool operator!=(const NullableField& that) // copy assignment
	{
		return !operator==(that.primitiveValue);
	}

	bool operator!=(const PrimitiveType& value) // copy assignment
	{
		return !operator==(primitiveValue == value);
	}

	bool operator>(const NullableField& that) // copy assignment
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->primitiveValue > that.primitiveValue;
	}

	bool operator>(const PrimitiveType& value) // copy assignment
	{
		assertLHSNotNull(*this);
		return this->primitiveValue > value;
	}

	bool operator>=(const NullableField& that) // copy assignment
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->primitiveValue >= that.primitiveValue;
	}

	bool operator>=(const PrimitiveType& value) // copy assignment
	{
		assertLHSNotNull(*this);
		return this->primitiveValue >= value;
	}

	bool operator<(const NullableField& that) // copy assignment
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->primitiveValue < that.primitiveValue;
	}

	bool operator<(const PrimitiveType& value) // copy assignment
	{
		assertLHSNotNull(*this);
		return this->primitiveValue < value;
	}

	bool operator<=(const NullableField& that) // copy assignment
	{
		assertLHSNotNull(*this);
		assertRHSNotNull(that);
		return this->primitiveValue <= that.primitiveValue;
	}

	bool operator<=(const PrimitiveType& value) // copy assignment
	{
		assertLHSNotNull(*this);
		return this->primitiveValue <= value;
	}

	operator PrimitiveType&(){
		return primitiveValue;
	}

	operator PrimitiveType(){
		return primitiveValue;
	}

	PrimitiveType& get(){
		return primitiveValue;
	}

	void setValue(const PrimitiveType& value){
		primitiveValue = value;
		hasValue = true;
	}

	std::string toString(){
		std::stringstream ss;
		ss << primitiveValue;
		return ss.str();
	}
};

typedef NullableField<std::string> String;
typedef NullableField<int> Integer;

const NullableFieldBase NullValue;
}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
