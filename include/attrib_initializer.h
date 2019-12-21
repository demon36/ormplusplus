#ifndef INCLUDE_ATTRIB_INITIALIZER_H_
#define INCLUDE_ATTRIB_INITIALIZER_H_

#include "attrib_initializer_base.h"
#include "nullable_field.h"

namespace ormplusplus{

/**
 * used at initialization of user model attributes, attrib_type is one of nullable_field<T> specializations<br>
 * ex: db_string, db_int, db_long<br>
 * 	responsible for creating a table_column statically in user_model & initializing a nullable field value for the attribute
 */
template<class attrib_type>
class attrib_initializer : public attrib_initializer_base{
private:
	attrib_type nfield;
public:
	attrib_initializer(attrib_initializer& that) = delete;

	/**
	 * @param _nfbase reference to field to be initialized
	 * @param _associated_col pointer to associated column to be initialized, nullptr in case it is already setup
	 */
	attrib_initializer(nullable_field_base& _nfbase, table_column* _associated_col)
	: attrib_initializer_base(_nfbase, _associated_col), nfield(_nfbase)
	{
	}

	//TODO: since this is the only thing that implied making this class a template class
	//		we can just make 2 variants of it, a safe variant with template argument
	//		and another unsafe variant, this will -probably- eliminate the need for
	//		making this class a template class
	//TODO: get rid of the need to supply a template parameter to this function
	//		since the attribute type is already provided
	template<class primitive_type>
	attrib_initializer& with_default(primitive_type value) {
		//TODO: check operator= is implemented correctly
		nfield = value;
		attrib_initializer_base::with_default(nfield.to_string());
		return *this;
	}

	//TODO: add more supported types for auto increment
	template < class T = attrib_type,
	           class = typename std::enable_if<std::is_same<T, db_int>::value || std::is_same<T, db_long>::value>::type>
	attrib_initializer& auto_increment() {
		attrib_initializer_base::auto_increment();
		return *this;
	}

	//TODO: throw exception if a decorator got called twice
	attrib_initializer& as_primary() {
		attrib_initializer_base::as_primary();
		return *this;
	}

	//todo: why is this -and others- overriden ?
	attrib_initializer& as_nullable(bool value = true) {
		attrib_initializer_base::as_nullable();
		return *this;
	}

	~attrib_initializer(){
	}

//	operator attrib_type&&(){
//		//FIXME: which recipient ctor will be called here ?
//		return std::move(nfield);
//	}
};

}
#endif /* INCLUDE_ATTRIB_INITIALIZER_H_ */
