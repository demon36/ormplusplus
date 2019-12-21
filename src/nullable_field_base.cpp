#include <algorithm>
#include <chrono>

#include "nullable_field_base.h"
#include "nullable_field.h"

using namespace std;

namespace ormplusplus {

ostream& operator<<(ostream& outstream, nullptr_t value){
	return outstream;
}

nullable_field_base::nullable_field_base()
: type_info_ref(Null::getTypeInfo())
{}

nullable_field_base::nullable_field_base(const type_info& typeInfo)
: type_info_ref(typeInfo)
{
	if(type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		primitive_value_ptr = new int(0);
	}else if(type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		primitive_value_ptr = new long(0);
	}else if(type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		primitive_value_ptr = new float(0);
	}else if(type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		primitive_value_ptr = new double(0);
	}else if(type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		primitive_value_ptr = new string("");
	}else if(type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
		primitive_value_ptr = new ::tm();
	}else if(type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitive_value_ptr = new nullptr_t();
	}else{
		throw runtime_error("unsupported data type at NullableFieldBase construction");
	}
}

nullable_field_base::nullable_field_base(const nullable_field_base& that)
: type_info_ref(that.type_info_ref), has_value(that.has_value)
{
	if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		primitive_value_ptr = new int(that.getValueRef<int>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		primitive_value_ptr = new long(that.getValueRef<long>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		primitive_value_ptr = new float(that.getValueRef<float>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		primitive_value_ptr = new double(that.getValueRef<double>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		primitive_value_ptr = new string(that.getValueRef<string>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
		primitive_value_ptr = new ::tm(that.getValueRef<::tm>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitive_value_ptr = new nullptr_t(that.getValueRef<nullptr_t>());
	}
}

nullable_field_base::nullable_field_base(nullable_field_base&& that)
: type_info_ref(that.type_info_ref), has_value(that.has_value)
{
	this->primitive_value_ptr = that.primitive_value_ptr;
	that.has_value = false;
	that.primitive_value_ptr = nullptr;
}

nullable_field_base& nullable_field_base::operator=(const nullable_field_base& that){
	if(this->type_info_ref != that.type_info_ref){
		throw runtime_error("calling NullableFieldBase::operator= with non-equal nullable field types");
	}else{
		this->has_value = that.has_value;
		if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
			this->getValueRef<int>() = that.getValueRef<int>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){
			this->getValueRef<long>() = that.getValueRef<long>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
			this->getValueRef<float>() = that.getValueRef<float>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
			this->getValueRef<double>() = that.getValueRef<double>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
			this->getValueRef<string>() = that.getValueRef<string>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
			this->getValueRef<::tm>() = that.getValueRef<::tm>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
			//is this the best way to do it ?
			this->getValueRef<nullptr_t>() = that.getValueRef<nullptr_t>();
		}
		return *this;
	}
}

bool nullable_field_base::equals(const nullable_field_base& that) const{
	if(this->type_info_ref != that.type_info_ref){
		return false;
	}

	if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		return this->getValueRef<int>() == that.getValueRef<int>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		return this->getValueRef<long>() == that.getValueRef<long>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		return this->getValueRef<float>() == that.getValueRef<float>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		return this->getValueRef<double>() == that.getValueRef<double>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		return this->getValueRef<string>() == that.getValueRef<string>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(tm).hash_code()){
		tm& lhs = this->getValueRef<tm>();
		tm& rhs = that.getValueRef<tm>();
		return	lhs.tm_year == rhs.tm_year &&
				lhs.tm_mon == rhs.tm_mon &&
				lhs.tm_mday == rhs.tm_mday &&
				lhs.tm_yday == rhs.tm_yday &&
				lhs.tm_hour == rhs.tm_hour &&
				lhs.tm_min == rhs.tm_min &&
				lhs.tm_sec == rhs.tm_sec;
	}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		return this->getValueRef<nullptr_t>() == that.getValueRef<nullptr_t>();
	}else{
		throw runtime_error("called NullableFieldBase::equals() with unsupported type");
	}

}

bool nullable_field_base::is_null() const {
	return !has_value;
}

string nullable_field_base::to_string() const
{
	if(is_null()){
		return "NULL";
	}
	if(type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		return to_string(*(int*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		return to_string(*(long*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		return to_string(*(float*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		return to_string(*(double*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		return *(string*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
		::tm& date = *(::tm*)primitive_value_ptr;
	    char dateBuffer[100] = {0};
	    std::strftime(dateBuffer, sizeof(dateBuffer), "%F %X", &date);
		return dateBuffer;
	}else if(type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		return "NULL";
	}
	throw runtime_error("unresolved type");
}

const type_info& nullable_field_base::get_type() const{
	return type_info_ref;
}

bool nullable_field_base::is_integral(const std::type_info& type){
	return type == typeid(int) || type == typeid(long) || type == typeid(double) || type == typeid(float);
}

bool nullable_field_base::is_text(const std::type_info& type){
	return type == typeid(string);//todo: is blob text ?
}

nullable_field_base::~nullable_field_base(){
	if(primitive_value_ptr == nullptr){
		return;
	}

	if(type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		delete (int*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		delete (long*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		delete (float*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		delete (double*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		delete (string*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
		delete (::tm*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		delete (nullptr_t*)primitive_value_ptr;
	}
}

} /* namespace ORMPlusPlus */
