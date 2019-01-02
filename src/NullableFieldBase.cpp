#include "NullableFieldBase.h"

namespace ORMPlusPlus {

std::ostream& operator<<(std::ostream& outstream, nullptr_t value){
	return outstream;
}

void NullableFieldBase::assertLHSNotNull(const NullableFieldBase& lhs){
	if(lhs.isNull()){
		throw std::runtime_error("comparing with null left hand operand");
	}
}

void NullableFieldBase::assertRHSNotNull(const NullableFieldBase& rhs){
	if(rhs.isNull()){
		throw std::runtime_error("comparing with null left hand operand");
	}
}

NullableFieldBase::NullableFieldBase()
: m_type(typeid(nullptr_t))
{}

NullableFieldBase::NullableFieldBase(const std::type_info& type)
: m_type(type)
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

NullableFieldBase::NullableFieldBase(const NullableFieldBase& that)
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

NullableFieldBase::NullableFieldBase(NullableFieldBase&& that)
: m_type(that.m_type), hasValue(that.hasValue)
{
	this->primitiveValuePtr = that.primitiveValuePtr;
	that.primitiveValuePtr = nullptr;
}

bool NullableFieldBase::isNull() const {
	return !hasValue;
}

std::string NullableFieldBase::toString()
{
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

NullableFieldBase::~NullableFieldBase(){
	if(primitiveValuePtr == nullptr){
		return;
	}

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

} /* namespace ORMPlusPlus */
