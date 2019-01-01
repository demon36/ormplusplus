#ifndef INCLUDE_NULLABLEFIELDBASE_H_
#define INCLUDE_NULLABLEFIELDBASE_H_

#include <string>
#include <sstream>
#include <iostream>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>


namespace ORMPlusPlus {

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
std::ostream& operator<<(std::ostream& outstream, nullptr_t value);

//possible to use type_info instead ?
enum DataType{
	_Integer,
	_Long,
	_Float,
	_Double,
	_String,
	_DateTime,
	_UnsupportedType,
};

class NullableFieldBase{
private:
	void* primitiveValuePtr = nullptr;
	const std::type_info& m_type;
	bool hasValue = false;
protected:
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
	template<class PrimitiveType>
	static NullableFieldBase create(const PrimitiveType& value){
		NullableFieldBase instance(typeid(PrimitiveType));
		instance.setValue<PrimitiveType>(value);
		return instance;
	}

	NullableFieldBase() : m_type(typeid(nullptr_t))
	{}

	NullableFieldBase(const std::type_info& type) : m_type(type)
	{
		std::cerr<<"NullableFieldBase type ctor"<<type.name()<<std::endl;
		if(m_type == typeid(int)){
			primitiveValuePtr = new int;
		}else if(m_type == typeid(long)){
			primitiveValuePtr = new long;
		}else if(m_type == typeid(float)){
			primitiveValuePtr = new float;
		}else if(m_type == typeid(double)){
			primitiveValuePtr = new double;
		}else if(m_type == typeid(std::string)){
			primitiveValuePtr = new std::string;
		}else if(m_type == typeid(Poco::DateTime)){
			primitiveValuePtr = new Poco::DateTime;
		}else if(m_type == typeid(nullptr_t)){
			//is this the best way to do it ?
			primitiveValuePtr = new nullptr_t();
		}else{
			throw std::runtime_error("unsupported data type at NullableFieldBase construction");
		}
	}

	NullableFieldBase(const NullableFieldBase& that)
	: m_type(that.m_type), hasValue(that.hasValue)
	{
		if(isNull()){
			return;
		}
		std::cerr<<"NullableFieldBase copy ctor"<<that.m_type.name()<<std::endl;
		if(that.m_type == typeid(int)){
			primitiveValuePtr = new int(that.getValueRef<int>());
		}else if(that.m_type == typeid(long)){
			primitiveValuePtr = new long(that.getValueRef<long>());
		}else if(that.m_type == typeid(float)){
			primitiveValuePtr = new float(that.getValueRef<float>());
		}else if(that.m_type == typeid(double)){
			primitiveValuePtr = new double(that.getValueRef<double>());
		}else if(that.m_type == typeid(std::string)){
			primitiveValuePtr = new std::string(that.getValueRef<std::string>());
		}else if(that.m_type == typeid(Poco::DateTime)){
			primitiveValuePtr = new Poco::DateTime(that.getValueRef<Poco::DateTime>());
		}else if(that.m_type == typeid(nullptr_t)){
			//is this the best way to do it ?
			primitiveValuePtr = new nullptr_t(that.getValueRef<nullptr_t>());
		}
	}

	NullableFieldBase(NullableFieldBase&& that)
	: m_type(that.m_type), hasValue(that.hasValue)
	{
		this->primitiveValuePtr = that.primitiveValuePtr;
		that.primitiveValuePtr = nullptr;
	}

	template<class PrimitiveType>
	const PrimitiveType& getValueRef() const{
		//TODO: assert not null
		if(typeid(PrimitiveType) == m_type){
			return *(PrimitiveType*)primitiveValuePtr;
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::getValueRef()");
		}
	}

	template<class PrimitiveType>
	void setValue(const PrimitiveType& value){
		//TODO: assert not null
		if(typeid(PrimitiveType) == m_type){
			*(PrimitiveType*)primitiveValuePtr = value;
			hasValue = true;
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::getValueRef()");
		}
	}

	~NullableFieldBase(){
		if(primitiveValuePtr == nullptr){
			return;
		}

		std::cerr<<"~NullableFieldBase of "<<toString()<<std::endl;

		if(m_type == typeid(int)){
			delete (int*)primitiveValuePtr;
		}else if(m_type == typeid(long)){
			delete (long*)primitiveValuePtr;
		}else if(m_type == typeid(float)){
			delete (float*)primitiveValuePtr;
		}else if(m_type == typeid(double)){
			delete (double*)primitiveValuePtr;
		}else if(m_type == typeid(std::string)){
			delete (std::string*)primitiveValuePtr;
		}else if(m_type == typeid(Poco::DateTime)){
			delete (Poco::DateTime*)primitiveValuePtr;
		}else if(m_type == typeid(nullptr_t)){
			delete (nullptr_t*)primitiveValuePtr;
		}
	}

	template<class AttribType>
	static DataType deduceDataType(){
//		if (std::is_same<AttribType, Integer>::value) {
//			return DataType::_Integer;
//		}else if(std::is_same<AttribType, Long>::value){
//			return DataType::_Long;
//		}else if(std::is_same<AttribType, Float>::value){
//			return DataType::_Float;
//		}else if(std::is_same<AttribType, Double>::value){
//			return DataType::_Double;
//		}else if(std::is_same<AttribType, String>::value){
//			return DataType::_String;
//		}else if(std::is_same<AttribType, DateTime>::value){
//			return DataType::_DateTime;
//		}else{
//			throw std::runtime_error("Data type not supported");
//		}
	}

	std::string toString(){
		if(isNull()){
			return "";
		}
		std::stringstream tempStream;
		if(m_type == typeid(int)){
			tempStream << *(int*)primitiveValuePtr;
		}else if(m_type == typeid(long)){
			tempStream << *(long*)primitiveValuePtr;
		}else if(m_type == typeid(float)){
			tempStream << *(float*)primitiveValuePtr;
		}else if(m_type == typeid(double)){
			tempStream << *(double*)primitiveValuePtr;
		}else if(m_type == typeid(std::string)){
			tempStream << *(std::string*)primitiveValuePtr;
		}else if(m_type == typeid(Poco::DateTime)){
			Poco::DateTime& date = *(Poco::DateTime*)primitiveValuePtr;
			tempStream << Poco::DateTimeFormatter::format(date, "%Y-%m-%d %h:%M:%S");
		}else if(m_type == typeid(nullptr_t)){
			tempStream << *(nullptr_t*)primitiveValuePtr;
		}
		return tempStream.str();
	}

	bool isNull() const {
		return !hasValue;
	}
};


} /* namespace ORMPlusPlus */

#endif /* INCLUDE_NULLABLEFIELDBASE_H_ */
