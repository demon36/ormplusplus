#include "NullableFieldBase.h"
#include "NullableField.h"
#include <algorithm>

using namespace std;

namespace ORMPlusPlus {

ostream& operator<<(ostream& outstream, nullptr_t value){
	return outstream;
}

//key: type_info hash
const map<size_t, TypeInfo> NullableFieldBase::typeInfoMap({
	{typeid(Integer).hash_code(), {true, false, "INT"}},
	{typeid(Long).hash_code(), {true, false, "BIGINT"}},
	{typeid(Float).hash_code(), {true, false, "FLOAT"}},
	{typeid(Double).hash_code(), {true, false, "DOUBLE"}},
	{typeid(String).hash_code(), {false, true, "VARCHAR"}},
	{typeid(DateTime).hash_code(), {false, false, "DATETIME"}},
});

size_t NullableFieldBase::getTypeHash(string TypeDBName){
	std::transform(TypeDBName.begin(), TypeDBName.end(), TypeDBName.begin(), ::toupper);
	for(auto& typeInfo : typeInfoMap){
		if(typeInfo.second.DBName == TypeDBName){
			return typeInfo.first;
		}
	}
	throw runtime_error("type not found");
}

void NullableFieldBase::assertLHSNotNull(const NullableFieldBase& lhs){
	if(lhs.isNull()){
		throw runtime_error("comparing with null left hand operand");
	}
}

void NullableFieldBase::assertRHSNotNull(const NullableFieldBase& rhs){
	if(rhs.isNull()){
		throw runtime_error("comparing with null left hand operand");
	}
}

NullableFieldBase::NullableFieldBase()
: m_type(typeid(nullptr_t))
{}

NullableFieldBase::NullableFieldBase(const type_info& type)
: m_type(type)
{
	if(m_type == typeid(int)){
		primitiveValuePtr = new int;
	}else if(m_type == typeid(long)){
		primitiveValuePtr = new long;
	}else if(m_type == typeid(float)){
		primitiveValuePtr = new float;
	}else if(m_type == typeid(double)){
		primitiveValuePtr = new double;
	}else if(m_type == typeid(string)){
		primitiveValuePtr = new string;
	}else if(m_type == typeid(Poco::DateTime)){
		primitiveValuePtr = new Poco::DateTime;
	}else if(m_type == typeid(nullptr_t)){
		//is this the best way to do it ?
		primitiveValuePtr = new nullptr_t();
	}else{
		throw runtime_error("unsupported data type at NullableFieldBase construction");
	}
}

NullableFieldBase::NullableFieldBase(const NullableFieldBase& that)
: m_type(that.m_type), hasValue(that.hasValue)
{
	if(that.m_type == typeid(int)){
		primitiveValuePtr = new int(that.getValueRef<int>());
	}else if(that.m_type == typeid(long)){
		primitiveValuePtr = new long(that.getValueRef<long>());
	}else if(that.m_type == typeid(float)){
		primitiveValuePtr = new float(that.getValueRef<float>());
	}else if(that.m_type == typeid(double)){
		primitiveValuePtr = new double(that.getValueRef<double>());
	}else if(that.m_type == typeid(string)){
		primitiveValuePtr = new string(that.getValueRef<string>());
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

NullableFieldBase& NullableFieldBase::operator=(const NullableFieldBase that){
	if(this->m_type != that.m_type){
		throw runtime_error("calling NullableFieldBase::operator= with non-equal nullable field types");
	}else{
		this->hasValue = that.hasValue;
		if(that.m_type == typeid(int)){
			this->getValueRef<int>() = that.getValueRef<int>();
		}else if(that.m_type == typeid(long)){
			this->getValueRef<long>() = that.getValueRef<long>();
		}else if(that.m_type == typeid(float)){
			this->getValueRef<float>() = that.getValueRef<float>();
		}else if(that.m_type == typeid(double)){
			this->getValueRef<double>() = that.getValueRef<double>();
		}else if(that.m_type == typeid(string)){
			this->getValueRef<string>() = that.getValueRef<string>();
		}else if(that.m_type == typeid(Poco::DateTime)){
			this->getValueRef<Poco::DateTime>() = that.getValueRef<Poco::DateTime>();
		}else if(that.m_type == typeid(nullptr_t)){
			//is this the best way to do it ?
			this->getValueRef<nullptr_t>() = that.getValueRef<nullptr_t>();
		}
		return *this;
	}
}

bool NullableFieldBase::isNull() const {
	return !hasValue;
}

string NullableFieldBase::toString() const
{
	if(isNull()){
		return "";
	}
	stringstream tempStream;
	if(m_type == typeid(int)){
		tempStream << *(int*)primitiveValuePtr;
	}else if(m_type == typeid(long)){
		tempStream << *(long*)primitiveValuePtr;
	}else if(m_type == typeid(float)){
		tempStream << *(float*)primitiveValuePtr;
	}else if(m_type == typeid(double)){
		tempStream << *(double*)primitiveValuePtr;
	}else if(m_type == typeid(string)){
		tempStream << *(string*)primitiveValuePtr;
	}else if(m_type == typeid(Poco::DateTime)){
		Poco::DateTime& date = *(Poco::DateTime*)primitiveValuePtr;
		tempStream << Poco::DateTimeFormatter::format(date, "%Y-%m-%d %h:%M:%S");
	}else if(m_type == typeid(nullptr_t)){
		tempStream << *(nullptr_t*)primitiveValuePtr;
	}
	return tempStream.str();
}

const std::type_info& NullableFieldBase::getType() const{
	return m_type;
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
	}else if(m_type == typeid(string)){
		delete (string*)primitiveValuePtr;
	}else if(m_type == typeid(Poco::DateTime)){
		delete (Poco::DateTime*)primitiveValuePtr;
	}else if(m_type == typeid(nullptr_t)){
		delete (nullptr_t*)primitiveValuePtr;
	}
}

} /* namespace ORMPlusPlus */
