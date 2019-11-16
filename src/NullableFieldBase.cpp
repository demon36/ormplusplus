#include <algorithm>
#include <chrono>

#include "NullableFieldBase.h"
#include "NullableField.h"

using namespace std;

namespace ORMPlusPlus {

ostream& operator<<(ostream& outstream, nullptr_t value){
	return outstream;
}

//key: type_info hash
//todo: merge with the mapping in MySQLSession
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
: typeHash(typeid(nullptr_t).hash_code())
{}

NullableFieldBase::NullableFieldBase(size_t _typeHash)
: typeHash(_typeHash)
{
	if(typeHash == typeid(int).hash_code()){
		primitiveValuePtr = new int(0);
	}else if(typeHash == typeid(long).hash_code()){
		primitiveValuePtr = new long(0);
	}else if(typeHash == typeid(float).hash_code()){
		primitiveValuePtr = new float(0);
	}else if(typeHash == typeid(double).hash_code()){
		primitiveValuePtr = new double(0);
	}else if(typeHash == typeid(string).hash_code()){
		primitiveValuePtr = new string("");
	}else if(typeHash == typeid(::tm).hash_code()){
		primitiveValuePtr = new ::tm();
	}else if(typeHash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitiveValuePtr = new nullptr_t();
	}else{
		throw runtime_error("unsupported data type at NullableFieldBase construction");
	}
}

NullableFieldBase::NullableFieldBase(const NullableFieldBase& that)
: typeHash(that.typeHash), hasValue(that.hasValue)
{
	if(that.typeHash == typeid(int).hash_code()){
		primitiveValuePtr = new int(that.getValueRef<int>());
	}else if(that.typeHash == typeid(long).hash_code()){
		primitiveValuePtr = new long(that.getValueRef<long>());
	}else if(that.typeHash == typeid(float).hash_code()){
		primitiveValuePtr = new float(that.getValueRef<float>());
	}else if(that.typeHash == typeid(double).hash_code()){
		primitiveValuePtr = new double(that.getValueRef<double>());
	}else if(that.typeHash == typeid(string).hash_code()){
		primitiveValuePtr = new string(that.getValueRef<string>());
	}else if(that.typeHash == typeid(::tm).hash_code()){
		primitiveValuePtr = new ::tm(that.getValueRef<::tm>());
	}else if(that.typeHash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitiveValuePtr = new nullptr_t(that.getValueRef<nullptr_t>());
	}
}

NullableFieldBase::NullableFieldBase(NullableFieldBase&& that)
: typeHash(that.typeHash), hasValue(that.hasValue)
{
	this->primitiveValuePtr = that.primitiveValuePtr;
	that.hasValue = false;
	that.primitiveValuePtr = nullptr;
}

NullableFieldBase& NullableFieldBase::operator=(const NullableFieldBase that){
	if(this->typeHash != that.typeHash){
		throw runtime_error("calling NullableFieldBase::operator= with non-equal nullable field types");
	}else{
		this->hasValue = that.hasValue;
		if(that.typeHash == typeid(int).hash_code()){
			this->getValueRef<int>() = that.getValueRef<int>();
		}else if(that.typeHash == typeid(long).hash_code()){
			this->getValueRef<long>() = that.getValueRef<long>();
		}else if(that.typeHash == typeid(float).hash_code()){
			this->getValueRef<float>() = that.getValueRef<float>();
		}else if(that.typeHash == typeid(double).hash_code()){
			this->getValueRef<double>() = that.getValueRef<double>();
		}else if(that.typeHash == typeid(string).hash_code()){
			this->getValueRef<string>() = that.getValueRef<string>();
		}else if(that.typeHash == typeid(::tm).hash_code()){
			this->getValueRef<::tm>() = that.getValueRef<::tm>();
		}else if(that.typeHash == typeid(nullptr_t).hash_code()){
			//is this the best way to do it ?
			this->getValueRef<nullptr_t>() = that.getValueRef<nullptr_t>();
		}
		return *this;
	}
}

bool NullableFieldBase::equals(const NullableFieldBase& that) const{
	if(this->typeHash != that.typeHash){
		return false;
	}

	if(that.typeHash == typeid(int).hash_code()){
		return this->getValueRef<int>() == that.getValueRef<int>();
	}else if(that.typeHash == typeid(long).hash_code()){
		return this->getValueRef<long>() == that.getValueRef<long>();
	}else if(that.typeHash == typeid(float).hash_code()){
		return this->getValueRef<float>() == that.getValueRef<float>();
	}else if(that.typeHash == typeid(double).hash_code()){
		return this->getValueRef<double>() == that.getValueRef<double>();
	}else if(that.typeHash == typeid(string).hash_code()){
		return this->getValueRef<string>() == that.getValueRef<string>();
	}else if(that.typeHash == typeid(::tm).hash_code()){
		::tm& lhs = this->getValueRef<::tm>();
		::tm& rhs = that.getValueRef<::tm>();
//		return lhs.tm_gmtoff == rhs.tm_gmtoff &&
//				lhs.tm_year == rhs.tm_year &&
//				lhs.tm_mon == rhs.tm_mon &&
//				lhs.tm_mday == rhs.tm_mday &&
//				lhs.tm_yday == rhs.tm_yday &&
//				lhs.tm_hour == rhs.tm_hour &&
//				lhs.tm_min == rhs.tm_min &&
//				lhs.tm_sec == rhs.tm_sec &&
//				lhs.tm_isdst == rhs.tm_isdst;
		return difftime(mktime(&lhs), mktime(&rhs)) == 0;
	}else if(that.typeHash == typeid(nullptr_t).hash_code()){
		return this->getValueRef<nullptr_t>() == that.getValueRef<nullptr_t>();
	}else{
		throw runtime_error("called NullableFieldBase::equals() with unsupported type");
	}

}

bool NullableFieldBase::isNull() const {
	return !hasValue;
}

string NullableFieldBase::toString() const
{
	if(isNull()){
		return "NULL";
	}
	if(typeHash == typeid(int).hash_code()){
		return to_string(*(int*)primitiveValuePtr);
	}else if(typeHash == typeid(long).hash_code()){
		return to_string(*(long*)primitiveValuePtr);
	}else if(typeHash == typeid(float).hash_code()){
		return to_string(*(float*)primitiveValuePtr);
	}else if(typeHash == typeid(double).hash_code()){
		return to_string(*(double*)primitiveValuePtr);
	}else if(typeHash == typeid(string).hash_code()){
		return *(string*)primitiveValuePtr;
	}else if(typeHash == typeid(::tm).hash_code()){
		::tm& date = *(::tm*)primitiveValuePtr;
	    char dateBuffer[100] = {0};
	    std::strftime(dateBuffer, sizeof(dateBuffer), "%F %X", &date);
		return dateBuffer;
	}else if(typeHash == typeid(nullptr_t).hash_code()){
		return "NULL";
	}
	throw runtime_error("unresolved type");
}

size_t NullableFieldBase::getType() const{
	return typeHash;
}

NullableFieldBase::~NullableFieldBase(){
	if(primitiveValuePtr == nullptr){
		return;
	}

	if(typeHash == typeid(int).hash_code()){
		delete (int*)primitiveValuePtr;
	}else if(typeHash == typeid(long).hash_code()){
		delete (long*)primitiveValuePtr;
	}else if(typeHash == typeid(float).hash_code()){
		delete (float*)primitiveValuePtr;
	}else if(typeHash == typeid(double).hash_code()){
		delete (double*)primitiveValuePtr;
	}else if(typeHash == typeid(string).hash_code()){
		delete (string*)primitiveValuePtr;
	}else if(typeHash == typeid(::tm).hash_code()){
		delete (::tm*)primitiveValuePtr;
	}else if(typeHash == typeid(nullptr_t).hash_code()){
		delete (nullptr_t*)primitiveValuePtr;
	}
}

} /* namespace ORMPlusPlus */
