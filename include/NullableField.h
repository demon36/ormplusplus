#ifndef INCLUDE_NULLABLEFIELD_H_
#define INCLUDE_NULLABLEFIELD_H_

#include <string>
#include <sstream>

#include <Poco/DateTime.h>

namespace ORMPlusPlus{

template<class PrimitiveType>
class NullableField;

typedef NullableField<int> Integer;
typedef NullableField<long> Long;
typedef NullableField<float> Float;
typedef NullableField<double> Double;
typedef NullableField<std::string> String;
typedef NullableField<Poco::DateTime> DateTime;
typedef NullableField<nullptr_t> Null;

//needed for using NullableField<nullptr_t>
std::ostream& operator<<(std::ostream& outstream, nullptr_t value){
	return outstream;
}

enum DataType{
	_Integer,
	_Long,
	_Float,
	_Double,
	_String,
	_DateTime,
};

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
	template<class AttribType>
	static DataType deduceDataType(){
		if (std::is_same<AttribType, Integer>::value) {
			return DataType::_Integer;
		}else if(std::is_same<AttribType, Long>::value){
			return DataType::_Long;
		}else if(std::is_same<AttribType, Float>::value){
			return DataType::_Float;
		}else if(std::is_same<AttribType, Double>::value){
			return DataType::_Double;
		}else if(std::is_same<AttribType, String>::value){
			return DataType::_String;
		}else if(std::is_same<AttribType, DateTime>::value){
			return DataType::_DateTime;
		}else{
			throw std::runtime_error("Data type not supported");
		}
	}

	virtual std::string toString() = 0;

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

const Null NullValue(nullptr);

}



#endif /* INCLUDE_NULLABLEFIELD_H_ */
