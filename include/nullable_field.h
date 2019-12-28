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
public:
	//todo: decrease visibility
	primitive_type value;
	bool _is_null = true;
private:
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
	: value(that.value)
	{
		_is_null = false;
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
		//todo: probably replace with
		//attrib_germ.get_model_base_ref.swallow(attrib_germ, *this);
		if(attrib_germ.attrib_col.has_default_value()){
			//todo: replace this with sth from the old implementation
			std::string default_val = attrib_germ.attrib_col.get_default_value().val;
			//todo: unify this accross the code
			from_string(attrib_germ.attrib_col.get_default_value().val, value);
			_is_null = attrib_germ.attrib_col.get_default_value().is_null;
		}

//		static bool add_col_if_not_exists(const std::string& name, const type_info& type)
		attrib_germ.attrib_col.set_type_info(nullable_field<primitive_type>::get_type_info());
		if(attrib_germ.schema_ref.find(attrib_germ.attrib_col.get_name()) == attrib_germ.schema_ref.end()){
			attrib_germ.schema_ref.emplace(attrib_germ.attrib_col.get_name(), attrib_germ.attrib_col);
		}

		attrib_germ.model_base_ref.get_attribs().emplace(
				std::piecewise_construct,
				std::forward_as_tuple(attrib_germ.attrib_col.get_name()), std::forward_as_tuple(get_type_info(), &value, _is_null)
				).first->second;

	}

	nullable_field(const primitive_type& _value)
	: value(_value)
	{
		_is_null = false;
	}

	const primitive_type& get_value_ref(){
		return value;
	}

	nullable_field& operator=(const nullable_field& that)
	{
		value = that.value;
		_is_null = false;
		return *this;
	}

	nullable_field& operator=(const primitive_type& _value)
	{
		value = _value;
		_is_null = false;
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
		return !operator==(that.value);
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
		return to_string(value);
	}

	~nullable_field()
	{
		//todo: might need to inform model_base that is fields has been invalidated
	}

	bool is_null() const
	{
		return _is_null;
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
	is >> field.value;
	field.has_value = true;
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
