#ifndef INCLUDE_NULLABLE_FIELD_H_
#define INCLUDE_NULLABLE_FIELD_H_

#include "nullable_field_base.h"
#include <memory>
#include "attrib_initializer_base.h"
#include "type_info.h"

namespace ormplusplus{

/**
 * Facilitates interaction with the nullable field
 */
template <class primitive_type>
class nullable_field;

template<class primitive_type>
std::ostream& operator<<(std::ostream&, const nullable_field<primitive_type>&);
template<class primitive_type>
std::istream& operator>>(std::istream&, const nullable_field<primitive_type>&);

/**
 * this class can be used either as a shell for a NullableFieldBase that exists some where else <br>
 * or as a container for NullableFieldBase
 */
template<class primitive_type>
class nullable_field{
private:
	nullable_field_base* nfbase_ptr = nullptr;
	bool is_ptr_owner = false;//should be false if used as shell
	primitive_type& value_ref;//useful at debugging
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
					nullable_field_base::is_integral(typeid(primitive_type)),
					nullable_field_base::is_text(typeid(primitive_type))
				));
		}
		return *type_info_ptr;
	}

	nullable_field()
	: nfbase_ptr(new nullable_field_base(get_type_info())),
	  is_ptr_owner(true),
	  value_ref(nfbase_ptr->get_value_ref<primitive_type>())
	{
	}

	nullable_field(const nullable_field& that)
	: nfbase_ptr(new nullable_field_base(*that.nfbase_ptr)),
	  is_ptr_owner(true),
	  value_ref(nfbase_ptr->get_value_ref<primitive_type>())
	{
	}

	nullable_field(const nullable_field&& that)
	: nfbase_ptr(new nullable_field_base(std::move(*that.nfbase_ptr))),
	  is_ptr_owner(true),
	  value_ref(nfbase_ptr->get_value_ref<primitive_type>())
	{
	}

	nullable_field(nullable_field_base& nfbase) //todo: convert to NullableFieldBase&&
	: nfbase_ptr(&nfbase),
	  is_ptr_owner(false),
	  value_ref(nfbase_ptr->get_value_ref<primitive_type>())
	{
	}

	nullable_field(const attrib_initializer_base& attribInitializer)
	: nullable_field(attribInitializer.get_nfbase_ref())
	{
	}

	nullable_field(const primitive_type& value)
	: nullable_field()
	{
		nfbase_ptr->set_value<primitive_type>(value);
	}

	nullable_field& operator=(const nullable_field& that)
	{
		nfbase_ptr->operator=(*that.nfbase_ptr);
		return *this;
	}

	nullable_field& operator=(const primitive_type& value)
	{
		nfbase_ptr->set_value<primitive_type>(value);
		return *this;
	}

	bool operator==(const nullable_field& that) const
	{
		if(this->is_null() && that.is_null()){
			return true;
		}else if(this->is_null() || that.is_null()){
			return false;
		}else{
			return this->get_value_ref() == that.get_value_ref();
		}
	}

	bool operator==(const primitive_type& value) const
	{
		if(this->is_null()){
			return false;
		}else{
			return this->get_value_ref() == value;
		}
	}

	bool operator!=(const nullable_field& that) const
	{
		return !operator==(that.get_value_ref());
	}

	bool operator!=(const primitive_type& value) const
	{
		return !operator==(value);
	}

	bool operator>(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->get_value_ref() > that.get_value_ref();
	}

	bool operator>(const primitive_type& value) const
	{
		assert_lhs_not_null(*this);
		return this->get_value_ref() > value;
	}

	bool operator>=(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->get_value_ref() >= that.get_value_ref();
	}

	bool operator>=(const primitive_type& value) const
	{
		assert_lhs_not_null(*this);
		return this->get_value_ref() >= value;
	}

	bool operator<(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->get_value_ref() < that.get_value_ref();
	}

	bool operator<(const primitive_type& value) const
	{
		assert_lhs_not_null(*this);
		return this->get_value_ref() < value;
	}

	bool operator<=(const nullable_field& that) const
	{
		assert_lhs_not_null(*this);
		assert_rhs_not_null(that);
		return this->get_value_ref() <= that.get_value_ref();
	}

	bool operator<=(const primitive_type& value) const
	{
		assert_lhs_not_null(*this);
		return this->get_value_ref() <= value;
	}

	operator primitive_type()
	{
		return get_value_ref();
	}

	std::string to_string() const
	{
		return nfbase_ptr->to_string();
	}

	~nullable_field()
	{
		if(is_ptr_owner && nfbase_ptr != nullptr){
			delete nfbase_ptr;
		}
	}

	primitive_type& get_value_ref() const
	{
		return nfbase_ptr->get_value_ref<primitive_type>();
	}

	nullable_field_base& getBaseRef()
	{
		return *nfbase_ptr;
	}

	bool is_null() const
	{
		return nfbase_ptr->is_null();
	}

	friend std::ostream& operator<< <>(std::ostream&, const nullable_field&);
	friend std::istream& operator>> <>(std::istream&, const nullable_field&);

};

template<class primitive_type>
std::unique_ptr<type_info> nullable_field<primitive_type>::type_info_ptr;

template<class primitive_type>
std::ostream& operator<<(std::ostream& os, const nullable_field<primitive_type>& field)
{
	os << field.get_value_ref();
    return os;
}

template<class primitive_type>
std::istream& operator>>(std::istream& is, const nullable_field<primitive_type>& field)
{
	is >> field.get_value_ref();
    return is;
}

//remember to add the necessary item to typeInfoMap at adding new specializations
typedef nullable_field<int> Integer;
typedef nullable_field<long> Long;
typedef nullable_field<float> Float;
typedef nullable_field<double> Double;
typedef nullable_field<std::string> String;
typedef nullable_field<::tm> DateTime;
//TODO: is this specialization essential ?
typedef nullable_field<nullptr_t> Null;


//TODO: replace by an empty constructor in NullableField
//to be used like if x == Null() -maybe?-
//extern const Null NullValue(nullptr);

}



#endif /* INCLUDE_NULLABLE_FIELD_H_ */
