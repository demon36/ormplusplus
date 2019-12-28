#ifndef INCLUDE_ATTRIB_INITIALIZER_H_
#define INCLUDE_ATTRIB_INITIALIZER_H_

#include "model_base.h"

namespace ormplusplus{

/**
 * used at initialization of user model attributes, attrib_type is one of nullable_field<T> specializations<br>
 * ex: db_string, db_int, db_long<br>
 * 	responsible for creating a table_column statically in user_model & initializing a nullable field value for the attribute
 */
//todo: move implementation to attrib_initializer.cpp
class attrib_initializer{
//todo: decrease visibility where needed
public:
	table_column attrib_col;
	table_schema& schema_ref;
	model_base& model_base_ref;
	//denotes that the schema entry for this model property has been already added
	const bool skip_col_setup = false;
public:
//	attrib_initializer(attrib_initializer& that) = delete;

	/**
	 * @param _nfbase reference to field to be initialized
	 * @param _associated_col pointer to associated column to be initialized, nullptr in case it is already setup
	 */
	//TODO: use table_column& + bool or table_column*
	attrib_initializer(const std::string& col_name, table_schema& _schema_ref, model_base& _model_base_ref);

	//TODO: since this is the only thing that implied making this class a template class
	//		we can just make 2 variants of it, a safe variant with template argument
	//		and another unsafe variant, this will -probably- eliminate the need for
	//		making this class a template class
	//TODO: get rid of the need to supply a template parameter to this function
	//		since the attribute type is already provided
	//TODO: when you enable using this class directly you would have to create more overloads for this
	//		function and/or parse string value to int/float/etc.. and set the nfbase value as well
	/*
	template<class primitive_type>
	attrib_initializer& with_default(primitive_type value) {
		//TODO: check operator= is implemented correctly
		if(!skip_col_setup){
			attrib_col.set_default_value(value);
		}
		return *this;
	}
	*/

	//todo: add variants
	attrib_initializer& with_default(const std::string& value);

	//TODO: add more supported types for auto increment
	/*
	template < class T = attrib_type,
	           class = typename std::enable_if<std::is_same<T, db_int>::value || std::is_same<T, db_long>::value>::type>
   */
	attrib_initializer& auto_increment();

	//TODO: throw exception if a decorator got called twice
	//TODO: make sure there is only one primary -maybe?-
	//TODO: make sure that primary keys are not nullable
	attrib_initializer& as_primary();

	//TODO: add support for non primary key index columns
	//attrib_initializer& as_index();//change column_defs, does it apply on numerical types only?

	attrib_initializer& as_nullable(bool value = true);

	~attrib_initializer();

//	operator attrib_type&&(){
//		//FIXME: which recipient ctor will be called here ?
//		return std::move(nfield);
//	}
};

}
#endif /* INCLUDE_ATTRIB_INITIALIZER_H_ */
