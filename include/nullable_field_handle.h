#ifndef INCLUDE_NULLABLE_FIELD_HANDLE_H_
#define INCLUDE_NULLABLE_FIELD_HANDLE_H_

#include <cstddef>
#include <string>
#include <sstream>
#include <typeindex>
#include <ostream>
#include "type_info.h"

namespace ormplusplus {

//TODO: remove this after removing nullable_field<nullptr_t>
//needed for using nullable_field<nullptr_t>

/**
 * Encapsulates the nullable field data
 */
class nullable_field_handle{
private:
	void* primitive_value_ptr = nullptr;
	bool* is_null_ptr = nullptr;
	const type_info& type_info_ref;

public:
	nullable_field_handle(const type_info& type, void* _primitive_value_ptr, bool* _is_null_ptr);
	nullable_field_handle(const nullable_field_handle& that);
	nullable_field_handle(nullable_field_handle&& that) = default;
	nullable_field_handle& operator=(const nullable_field_handle& that);

	static void move(nullable_field_handle& src, nullable_field_handle& dest);

	template<class primitive_type>
	nullable_field_handle& operator=(const primitive_type& value){
		//TODO: add fn assert type ?
		if(typeid(primitive_type).hash_code() != type_info_ref.primitive_type_hash){//todo: fix assertion done twice
			throw std::runtime_error("trying to assign nullable field to non-compatible type value");
		}
		get_value_ref<primitive_type>() = value;
		(*is_null_ptr) = false;
		return *this;
	}

	template<class primitive_type>
	primitive_type& get_value_ref() const{
		//TODO: assert not null
		if(typeid(primitive_type).hash_code() == type_info_ref.primitive_type_hash){
			return *(primitive_type*)primitive_value_ptr;
		}else{
			throw std::runtime_error("type mismatch at nullable_field_base::get_value_ref()");
		}
	}

	template<class primitive_type>
	void set_value(const primitive_type& value){
		//TODO: assert not null
		//TODO: use enable if
		if(typeid(primitive_type).hash_code() == type_info_ref.primitive_type_hash){
			*(primitive_type*)primitive_value_ptr = value;
			(*is_null_ptr) = false;
		}else{
			throw std::runtime_error("type mismatch at nullable_field_base::set_value(value)");
		}
	}

	void clear_value();

	template<class primitive_type>
	void set_value_unsafe(const primitive_type& value){
		*(primitive_type*)primitive_value_ptr = value;
		(*is_null_ptr) = false;
	}

	bool equals(const nullable_field_handle& that) const;
	bool is_null() const;
	std::string to_string() const;
	const type_info& get_type() const;
	static bool is_integral(const std::type_info& type);
	static bool is_text(const std::type_info& type);

	//TODO: support bypassing to streams
	~nullable_field_handle();
};

} /* namespace ormplusplus */

#endif /* INCLUDE_NULLABLE_FIELD_HANDLE_H_ */
