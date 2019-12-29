#ifndef INCLUDE_NULLABLE_FIELD_H_
#define INCLUDE_NULLABLE_FIELD_H_

#include <memory>
#include <type_traits>

#include "attrib_initializer.h"
#include "nullable_field_handle.h"
#include "type_info.h"

namespace ormplusplus{

/**
 * Facilitates interaction with the nullable field
 */
//todo: remove unnecessary fw declarations
template <class primitive_type>
class nullable_field;

template<class primitive_type>
std::ostream& operator<<(std::ostream&, const nullable_field<primitive_type>&);
template<class primitive_type>
std::istream& operator>>(std::istream&, const nullable_field<primitive_type>&);

/**
 * this class can be used either as a shell for a nullable_field_base that exists some where else <br>
 * or as a container for nullable_field_base
 */
template<class primitive_type>
class nullable_field{
private:
	primitive_type value;
	bool m_is_null = true;
	static std::unique_ptr<type_info> type_info_ptr;

	static void assert_lhs_not_null(const nullable_field& lhs){
		if (lhs.is_null()) {
			throw std::runtime_error("comparing with null left hand operand");
		}
	}

	static void assert_rhs_not_null(const nullable_field& rhs){
		if (rhs.is_null()) {
			throw std::runtime_error("comparing with null left hand operand");
		}
	}

public:
	static const type_info& get_type_info(){
		if(type_info_ptr == nullptr){
			type_info_ptr.reset(new type_info(
					typeid(nullable_field<primitive_type>),
					typeid(primitive_type),
					nullable_field_handle::is_integral(typeid(primitive_type)),
					nullable_field_handle::is_text(typeid(primitive_type))
				));
		}
		return *type_info_ptr;
	}

	nullable_field()
	{
	}

	nullable_field(const nullable_field& that)
	{
		if(that.is_null()){
			this->clear_value();
		}else{
			this->set_value(that.value);
		}
	}

	void clear_value(){
		m_is_null = true;
	}

	void set_value(const primitive_type& _value){
		value = _value;
		m_is_null = false;
	}

	//todo: is this safe ?
	nullable_field(nullable_field&& that) = default;

	/*
	nullable_field(nullable_field_base& nfbase) //todo: convert to nullable_field_base&&
	: nfbase_ptr(&nfbase),
	  is_ptr_owner(false),
	  value_ref(nfbase_ptr->get_value_ref<primitive_type>())
	{
	}
	*/

	static void from_string(const std::string& src, int& dest)			{ dest = stoi(src); }
	static void from_string(const std::string& src, long& dest)			{ dest = stol(src); }
	static void from_string(const std::string& src, float& dest)		{ dest = stof(src); }
	static void from_string(const std::string& src, double& dest)		{ dest = stod(src); }
	static void from_string(const std::string& src, std::string& dest)	{ dest = src; }
	static void from_string(const std::string& src, tm& dest)			{ strptime(src.c_str(), "%Y-%m-%d %H:%M:%S", &dest); }

	nullable_field(attrib_initializer attrib_germ)
	{
		if(attrib_germ.has_default_value()){
			//todo: replace this with sth from the old implementation
			//todo: unify this accross the code
			if(attrib_germ.get_default_value().is_null){
				m_is_null = true;
			} else {
				std::string default_val = attrib_germ.get_default_value().val;
				from_string(attrib_germ.get_default_value().val, value);
				m_is_null = false;
			}
		}
		attrib_germ.initialize_field(nullable_field<primitive_type>::get_type_info(), &value, &m_is_null);//the missing ring
	}

	nullable_field(const primitive_type& _value)
	: value(_value), m_is_null(false)
	{
	}

	const primitive_type& get_value_ref(){
		return value;
	}

	nullable_field_handle get_handle(){
		return {nullable_field<primitive_type>::get_type_info(), &value, &m_is_null};
	}

	nullable_field& operator=(const nullable_field& that)
	{
		if(that.is_null()){
			this->clear_value();
		}else{
			this->set_value(that.value);
		}
		return *this;
	}

	nullable_field& operator=(const primitive_type& _value)
	{
		this->set_value(_value);
		return *this;
	}

	bool operator==(const nullable_field& that) const
	{
		if(this->is_null() && that.is_null()){
			return true;
		}else if(this->is_null() || that.is_null()){
			return false;
		}else{
			return this->value == that.value;
		}
	}

	bool operator==(const primitive_type& _value) const
	{
		if(this->is_null()){
			return false;
		}else{
			return this->value == _value;
		}
	}

	bool operator!=(const nullable_field& that) const
	{
		return !operator==(that);
	}

	bool operator!=(const primitive_type& _value) const
	{
		return !operator==(_value);
	}

	bool operator>(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->value > that.value;
	}

	bool operator>(const primitive_type& _value) const
	{
		assert_lhs_not_null(*this);
		return this->value > _value;
	}

	bool operator>=(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->value >= that.value;
	}

	bool operator>=(const primitive_type& _value) const
	{
		assert_lhs_not_null(*this);
		return this->value >= _value;
	}

	bool operator<(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->value < that.value;
	}

	bool operator<(const primitive_type& _value) const
	{
		assert_lhs_not_null(*this);
		return this->value < _value;
	}

	bool operator<=(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->value <= that.value;
	}

	bool operator<=(const primitive_type& _value) const
	{
		assert_lhs_not_null(*this);
		return this->value <= _value;
	}

	operator primitive_type()
	{
		return value;
	}

	std::string to_string() const
	{
		return to_string(*value);
	}

	~nullable_field()
	{
		//todo: might need to inform model_base that is fields has been invalidated
	}

	bool is_null() const
	{
		return m_is_null;
	}

	friend std::ostream& operator<< <>(std::ostream&, const nullable_field&);
	friend std::istream& operator>> <>(std::istream&, const nullable_field&);

};

template<class primitive_type>
std::unique_ptr<type_info> nullable_field<primitive_type>::type_info_ptr;

template<class primitive_type>
std::ostream& operator<<(std::ostream& os, const nullable_field<primitive_type>& field)
{
	os << field.value;
    return os;
}

template<class primitive_type>
std::istream& operator>>(std::istream& is, const nullable_field<primitive_type>& field)
{
	primitive_type temp_value;//todo: optimize this
	is >> temp_value;
	field.set_value(temp_value);
    return is;
}

//remember to add conditions to nullable_field_base::is_integral and nullable_field_base::is_text at adding new specializations
typedef nullable_field<int> db_int;
typedef nullable_field<long> db_long;
typedef nullable_field<float> db_float;
typedef nullable_field<double> db_double;
typedef nullable_field<std::string> db_string;
typedef nullable_field<::tm> db_datetime;
//TODO: is this specialization essential ?
typedef nullable_field<nullptr_t> db_null;

//TODO: replace by an empty constructor in nullable_field
//to be used like if x == Null() -maybe?-
extern const db_null null_value;

}



#endif /* INCLUDE_NULLABLE_FIELD_H_ */
