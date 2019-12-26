#include <nullable_field_handle.h>
#include <algorithm>
#include <chrono>

#include "nullable_field.h"

using namespace std;

namespace ormplusplus {

ostream& operator<<(ostream& outstream, nullptr_t value){
	return outstream;
}

/*
nullable_field_handle::nullable_field_handle()
: type_info_ref(db_null::get_type_info()), has_value(null_value._is_null)
{}
*/

nullable_field_handle::nullable_field_handle(const type_info& type, void* _primitive_value_ptr, bool& _has_value)
: primitive_value_ptr(_primitive_value_ptr), type_info_ref(type), has_value(_has_value)
{
}

nullable_field_handle nullable_field_handle::create(const type_info& type, void* _primitive_value_ptr, bool _has_value){
	nullable_field_handle instance(type, _primitive_value_ptr, _has_value);
	return instance;
}

void destroy_unsafe(){
}

/*
nullable_field_handle::nullable_field_handle(const type_info& type_info)
: type_info_ref(type_info)
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
		throw runtime_error("unsupported data type at nullable_field_base construction");
	}
}
*/

nullable_field_handle::nullable_field_handle(const nullable_field_handle& that)
: type_info_ref(that.type_info_ref), has_value(that.has_value)
{
	if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		primitive_value_ptr = new int(that.get_value_ref<int>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		primitive_value_ptr = new long(that.get_value_ref<long>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		primitive_value_ptr = new float(that.get_value_ref<float>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		primitive_value_ptr = new double(that.get_value_ref<double>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		primitive_value_ptr = new string(that.get_value_ref<string>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
		primitive_value_ptr = new ::tm(that.get_value_ref<::tm>());
	}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		//is this the best way to do it ?
		primitive_value_ptr = new nullptr_t(that.get_value_ref<nullptr_t>());
	}
}

nullable_field_handle::nullable_field_handle(nullable_field_handle&& that)
: type_info_ref(that.type_info_ref), has_value(that.has_value)
{
	this->primitive_value_ptr = that.primitive_value_ptr;
	that.has_value = false;
	that.primitive_value_ptr = nullptr;
}

nullable_field_handle& nullable_field_handle::operator=(const nullable_field_handle& that){
	if(this->type_info_ref != that.type_info_ref){
		throw runtime_error("calling nullable_field_base::operator= with non-equal nullable field types");
	}else{
		this->has_value = that.has_value;
		if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
			this->get_value_ref<int>() = that.get_value_ref<int>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){
			this->get_value_ref<long>() = that.get_value_ref<long>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
			this->get_value_ref<float>() = that.get_value_ref<float>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
			this->get_value_ref<double>() = that.get_value_ref<double>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
			this->get_value_ref<string>() = that.get_value_ref<string>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
			this->get_value_ref<::tm>() = that.get_value_ref<::tm>();
		}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
			//is this the best way to do it ?
			this->get_value_ref<nullptr_t>() = that.get_value_ref<nullptr_t>();
		}
		return *this;
	}
}

bool nullable_field_handle::equals(const nullable_field_handle& that) const{
	if(this->type_info_ref != that.type_info_ref){
		return false;
	}

	if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		return this->get_value_ref<int>() == that.get_value_ref<int>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		return this->get_value_ref<long>() == that.get_value_ref<long>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		return this->get_value_ref<float>() == that.get_value_ref<float>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		return this->get_value_ref<double>() == that.get_value_ref<double>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		return this->get_value_ref<string>() == that.get_value_ref<string>();
	}else if(that.type_info_ref.primitive_type_hash == typeid(tm).hash_code()){
		tm& lhs = this->get_value_ref<tm>();
		tm& rhs = that.get_value_ref<tm>();
		return	lhs.tm_year == rhs.tm_year &&
				lhs.tm_mon == rhs.tm_mon &&
				lhs.tm_mday == rhs.tm_mday &&
				lhs.tm_yday == rhs.tm_yday &&
				lhs.tm_hour == rhs.tm_hour &&
				lhs.tm_min == rhs.tm_min &&
				lhs.tm_sec == rhs.tm_sec;
	}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		return this->get_value_ref<nullptr_t>() == that.get_value_ref<nullptr_t>();
	}else{
		throw runtime_error("called nullable_field_base::equals() with unsupported type");
	}

}

bool nullable_field_handle::is_null() const {
	return !has_value;
}

string nullable_field_handle::to_string() const
{
	if(is_null()){
		return "NULL";
	}
	if(type_info_ref.primitive_type_hash == typeid(int).hash_code()){
		return std::to_string(*(int*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(long).hash_code()){
		return std::to_string(*(long*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(float).hash_code()){
		return std::to_string(*(float*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(double).hash_code()){
		return std::to_string(*(double*)primitive_value_ptr);
	}else if(type_info_ref.primitive_type_hash == typeid(string).hash_code()){
		return *(string*)primitive_value_ptr;
	}else if(type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
		::tm& date = *(::tm*)primitive_value_ptr;
	    char date_buffer[100] = {0};
	    std::strftime(date_buffer, sizeof(date_buffer), "%F %X", &date);
		return date_buffer;
	}else if(type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
		return "NULL";
	}
	throw runtime_error("unresolved type");
}

const type_info& nullable_field_handle::get_type() const{
	return type_info_ref;
}

bool nullable_field_handle::is_integral(const std::type_info& type){
	return type == typeid(int) || type == typeid(long) || type == typeid(double) || type == typeid(float);
}

bool nullable_field_handle::is_text(const std::type_info& type){
	return type == typeid(string);//todo: is blob text ?
}

nullable_field_handle::~nullable_field_handle(){
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

const db_null null_value(nullptr);

} /* namespace ormplusplus */
