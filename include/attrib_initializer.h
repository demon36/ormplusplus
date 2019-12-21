#ifndef INCLUDE_ATTRIB_INITIALIZER_H_
#define INCLUDE_ATTRIB_INITIALIZER_H_

#include "attrib_initializer_base.h"
#include "nullable_field.h"

namespace ormplusplus{

/**
 * used at initialization of user model attributes, AttribType is one of NullableField<T> specializations<br>
 * ex: String, Integer, Long<br>
 * 	responsible for creating a TableColumn statically in UserModel & initializing a nullable field value for the attribute
 */
template<class attrib_type>
class attrib_initializer : public attrib_initializer_base{
private:
	attrib_type nfield;
public:
	attrib_initializer(attrib_initializer& that) = delete;

	/**
	 * @param NFBase reference to field to be initialized
	 * @param associatedColumn pointer to associated column to be initialized, nullptr in case it is already setup
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
	attrib_initializer& with_default(primitive_type value) override {
		//TODO: check operator= is implemented correctly
		nfield = value;
		attrib_initializer_base::with_default(nfield.toString());
		return *this;
	}

	//TODO: add more supported types for auto increment
	template < class T = attrib_type,
	           class = typename std::enable_if<std::is_same<T, Integer>::value || std::is_same<T, Long>::value>::type>
	attrib_initializer& auto_increment() override {
		attrib_initializer_base::auto_increment();
		return *this;
	}

	//TODO: throw exception if a decorator got called twice
	attrib_initializer& as_primary() override {
		attrib_initializer_base::as_primary();
		return *this;
	}

	//todo: why is this -and others- overriden ?
	attrib_initializer& as_nullable(bool value = true) override {
		attrib_initializer_base::as_nullable();
		return *this;
	}

	~attrib_initializer(){
	}

//	operator AttribType&&(){
//		//FIXME: which recipient ctor will be called here ?
//		return std::move(nullableField);
//	}
};

}
#endif /* INCLUDE_ATTRIB_INITIALIZER_H_ */
