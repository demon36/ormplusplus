#ifndef INCLUDE_NULLABLE_FIELD_BASE_H_
#define INCLUDE_NULLABLE_FIELD_BASE_H_

#include <cstddef>
#include <string>
#include <sstream>
#include <typeindex>
#include "type_info.h"

namespace ormplusplus {

//TODO: remove this after removing nullable_field<nullptr_t>
//needed for using nullable_field<nullptr_t>
std::ostream& operator<<(std::ostream& outstream, nullptr_t value);

/**
 * Encapsulates the nullable field data
 */
class nullable_field_base{
private:
	void* primitive_value_ptr = nullptr;
	const type_info& type_info_ref;
	bool has_value = false;

public:
	template<class primitive_type>
	static nullable_field_base create(const type_info& type, const primitive_type& value){
		nullable_field_base instance(type);
		instance.set_value<primitive_type>(value);
		return instance;
	}

	nullable_field_base();
	nullable_field_base(const type_info& type);
	nullable_field_base(const nullable_field_base& that);
	nullable_field_base(nullable_field_base&& that);
	nullable_field_base& operator=(const nullable_field_base& that);

	template<class primitive_type>
	nullable_field_base& operator=(const primitive_type& value){
		//TODO: add fn assert type ?
		if(typeid(primitive_type).hash_code() != type_info_ref.primitive_type_hash){//todo: fix assertion done twice
			throw std::runtime_error("trying to assign nullable field to non-compatible type value");
		}
		get_value_ref<primitive_type>() = value;
		has_value = true;
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
			has_value = true;//todo: has_value has_value with value_ptr != nullptr
		}else{
			throw std::runtime_error("type mismatch at nullable_field_base::set_value(value)");
		}
	}

	bool equals(const nullable_field_base& that) const;
	bool is_null() const;
	std::string to_string() const;
	const type_info& get_type() const;
	static bool is_integral(const std::type_info& type);
	static bool is_text(const std::type_info& type);

	//TODO: support bypassing to streams
	~nullable_field_base();
};

} /* namespace ormplusplus */

#endif /* INCLUDE_NULLABLE_FIELD_BASE_H_ */
