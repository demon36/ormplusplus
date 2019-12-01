#include <algorithm>
#include <chrono>

#include "NullableFieldBase.h"
#include "NullableField.h"

using namespace std;

namespace ORMPlusPlus {

ostream& operator<<(ostream& outstream, nullptr_t value){
	return outstream;
}

NullableFieldBase::NullableFieldBase()
: typeInfoRef(Null::getTypeInfo())
{}

NullableFieldBase::NullableFieldBase(const TypeInfo& typeInfo)
: typeInfoRef(typeInfo)
{
	if(typeInfoRef.primitiveTypeHash == typeid(int).hash_code()){
		primitiveValuePtr = new int(0);
	}else if(typeInfoRef.primitiveTypeHash == typeid(long).hash_code()){
		primitiveValuePtr = new long(0);
	}else if(typeInfoRef.primitiveTypeHash == typeid(float).hash_code()){
		primitiveValuePtr = new float(0);
	}else if(typeInfoRef.primitiveTypeHash == typeid(double).hash_code()){
		primitiveValuePtr = new double(0);
	}else if(typeInfoRef.primitiveTypeHash == typeid(string).hash_code()){
		primitiveValuePtr = new string("");
	}else if(typeInfoRef.primitiveTypeHash == typeid(::tm).hash_code()){
		primitiveValuePtr = new ::tm();
	}else if(typeInfoRef.primitiveTypeHash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitiveValuePtr = new nullptr_t();
	}else{
		throw runtime_error("unsupported data type at NullableFieldBase construction");
	}
}

NullableFieldBase::NullableFieldBase(const NullableFieldBase& that)
: typeInfoRef(that.typeInfoRef), hasValue(that.hasValue)
{
	if(that.typeInfoRef.primitiveTypeHash == typeid(int).hash_code()){
		primitiveValuePtr = new int(that.getValueRef<int>());
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(long).hash_code()){
		primitiveValuePtr = new long(that.getValueRef<long>());
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(float).hash_code()){
		primitiveValuePtr = new float(that.getValueRef<float>());
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(double).hash_code()){
		primitiveValuePtr = new double(that.getValueRef<double>());
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(string).hash_code()){
		primitiveValuePtr = new string(that.getValueRef<string>());
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(::tm).hash_code()){
		primitiveValuePtr = new ::tm(that.getValueRef<::tm>());
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitiveValuePtr = new nullptr_t(that.getValueRef<nullptr_t>());
	}
}

NullableFieldBase::NullableFieldBase(NullableFieldBase&& that)
: typeInfoRef(that.typeInfoRef), hasValue(that.hasValue)
{
	this->primitiveValuePtr = that.primitiveValuePtr;
	that.hasValue = false;
	that.primitiveValuePtr = nullptr;
}

NullableFieldBase& NullableFieldBase::operator=(const NullableFieldBase& that){
	if(this->typeInfoRef != that.typeInfoRef){
		throw runtime_error("calling NullableFieldBase::operator= with non-equal nullable field types");
	}else{
		this->hasValue = that.hasValue;
		if(that.typeInfoRef.primitiveTypeHash == typeid(int).hash_code()){
			this->getValueRef<int>() = that.getValueRef<int>();
		}else if(that.typeInfoRef.primitiveTypeHash == typeid(long).hash_code()){
			this->getValueRef<long>() = that.getValueRef<long>();
		}else if(that.typeInfoRef.primitiveTypeHash == typeid(float).hash_code()){
			this->getValueRef<float>() = that.getValueRef<float>();
		}else if(that.typeInfoRef.primitiveTypeHash == typeid(double).hash_code()){
			this->getValueRef<double>() = that.getValueRef<double>();
		}else if(that.typeInfoRef.primitiveTypeHash == typeid(string).hash_code()){
			this->getValueRef<string>() = that.getValueRef<string>();
		}else if(that.typeInfoRef.primitiveTypeHash == typeid(::tm).hash_code()){
			this->getValueRef<::tm>() = that.getValueRef<::tm>();
		}else if(that.typeInfoRef.primitiveTypeHash == typeid(nullptr_t).hash_code()){
			//is this the best way to do it ?
			this->getValueRef<nullptr_t>() = that.getValueRef<nullptr_t>();
		}
		return *this;
	}
}

bool NullableFieldBase::equals(const NullableFieldBase& that) const{
	if(this->typeInfoRef != that.typeInfoRef){
		return false;
	}

	if(that.typeInfoRef.primitiveTypeHash == typeid(int).hash_code()){
		return this->getValueRef<int>() == that.getValueRef<int>();
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(long).hash_code()){
		return this->getValueRef<long>() == that.getValueRef<long>();
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(float).hash_code()){
		return this->getValueRef<float>() == that.getValueRef<float>();
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(double).hash_code()){
		return this->getValueRef<double>() == that.getValueRef<double>();
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(string).hash_code()){
		return this->getValueRef<string>() == that.getValueRef<string>();
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(tm).hash_code()){
		tm& lhs = this->getValueRef<tm>();
		tm& rhs = that.getValueRef<tm>();
		return	lhs.tm_year == rhs.tm_year &&
				lhs.tm_mon == rhs.tm_mon &&
				lhs.tm_mday == rhs.tm_mday &&
				lhs.tm_yday == rhs.tm_yday &&
				lhs.tm_hour == rhs.tm_hour &&
				lhs.tm_min == rhs.tm_min &&
				lhs.tm_sec == rhs.tm_sec;
	}else if(that.typeInfoRef.primitiveTypeHash == typeid(nullptr_t).hash_code()){
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
	if(typeInfoRef.primitiveTypeHash == typeid(int).hash_code()){
		return to_string(*(int*)primitiveValuePtr);
	}else if(typeInfoRef.primitiveTypeHash == typeid(long).hash_code()){
		return to_string(*(long*)primitiveValuePtr);
	}else if(typeInfoRef.primitiveTypeHash == typeid(float).hash_code()){
		return to_string(*(float*)primitiveValuePtr);
	}else if(typeInfoRef.primitiveTypeHash == typeid(double).hash_code()){
		return to_string(*(double*)primitiveValuePtr);
	}else if(typeInfoRef.primitiveTypeHash == typeid(string).hash_code()){
		return *(string*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(::tm).hash_code()){
		::tm& date = *(::tm*)primitiveValuePtr;
	    char dateBuffer[100] = {0};
	    std::strftime(dateBuffer, sizeof(dateBuffer), "%F %X", &date);
		return dateBuffer;
	}else if(typeInfoRef.primitiveTypeHash == typeid(nullptr_t).hash_code()){
		return "NULL";
	}
	throw runtime_error("unresolved type");
}

const TypeInfo& NullableFieldBase::getType() const{
	return typeInfoRef;
}

bool NullableFieldBase::isIntegral(const std::type_info& type){
	return type == typeid(int) || type == typeid(long) || type == typeid(double) || type == typeid(float);
}

bool NullableFieldBase::isText(const std::type_info& type){
	return type == typeid(string);//todo: is blob text ?
}

NullableFieldBase::~NullableFieldBase(){
	if(primitiveValuePtr == nullptr){
		return;
	}

	if(typeInfoRef.primitiveTypeHash == typeid(int).hash_code()){
		delete (int*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(long).hash_code()){
		delete (long*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(float).hash_code()){
		delete (float*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(double).hash_code()){
		delete (double*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(string).hash_code()){
		delete (string*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(::tm).hash_code()){
		delete (::tm*)primitiveValuePtr;
	}else if(typeInfoRef.primitiveTypeHash == typeid(nullptr_t).hash_code()){
		delete (nullptr_t*)primitiveValuePtr;
	}
}

} /* namespace ORMPlusPlus */
