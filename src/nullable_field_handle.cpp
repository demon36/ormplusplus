#include <algorithm>
#include <chrono>

#include "nullable_field_handle.h"

using namespace std;

namespace ormplusplus {

nullable_field_handle::nullable_field_handle(const type_info& type, void* _primitive_value_ptr, bool* _is_null_ptr)
: primitive_value_ptr(_primitive_value_ptr), is_null_ptr(_is_null_ptr), type_info_ref(type)
{
}

void nullable_field_handle::move(nullable_field_handle& src, nullable_field_handle& dest)
{
	if(dest.is_null()){
		src.clear_value();
	} else {
		src.primitive_value_ptr = dest.primitive_value_ptr;
	}
}

void nullable_field_handle::clear_value(){
	*is_null_ptr = true;
}

nullable_field_handle::nullable_field_handle(const nullable_field_handle& that)
: primitive_value_ptr(that.primitive_value_ptr), is_null_ptr(that.is_null_ptr), type_info_ref(that.type_info_ref)
{
}

nullable_field_handle& nullable_field_handle::operator=(const nullable_field_handle& that){
	if(this->type_info_ref != that.type_info_ref){
		throw runtime_error("calling nullable_field_base::operator= with non-equal nullable field types");
	}else if (that.is_null()){
		clear_value();
	}else{
		if(that.type_info_ref.primitive_type_hash == typeid(int).hash_code()){
			this->set_value_unsafe<int>(that.get_value_ref<int>());//todo: add copy_value<primitive_types>(src,dest)
		}else if(that.type_info_ref.primitive_type_hash == typeid(long).hash_code()){//todo: enhance comparison
			this->set_value_unsafe<long>(that.get_value_ref<long>());
		}else if(that.type_info_ref.primitive_type_hash == typeid(float).hash_code()){
			this->set_value_unsafe<float>(that.get_value_ref<float>());
		}else if(that.type_info_ref.primitive_type_hash == typeid(double).hash_code()){
			this->set_value_unsafe<double>(that.get_value_ref<double>());
		}else if(that.type_info_ref.primitive_type_hash == typeid(string).hash_code()){
			this->set_value_unsafe<string>(that.get_value_ref<string>());//todo: use bare char*
		}else if(that.type_info_ref.primitive_type_hash == typeid(::tm).hash_code()){
			this->set_value_unsafe<tm>(that.get_value_ref<tm>());
		}else if(that.type_info_ref.primitive_type_hash == typeid(nullptr_t).hash_code()){
			//is this the best way to do it ?
			this->get_value_ref<nullptr_t>() = that.get_value_ref<nullptr_t>();
		}
	}
	return *this;
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
	return *is_null_ptr;
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
	    std::strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d %X", &date);
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
}

//const db_null null_value(nullptr);

} /* namespace ormplusplus */
