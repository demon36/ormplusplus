#include "attrib_initializer_base.h"
#include "table_column.h"

namespace ormplusplus {

attrib_initializer_base::attrib_initializer_base(nullable_field_base& _nfbase, table_column* associated_col)
: nfbase(_nfbase), attrib_col_ptr(associated_col), skip_col_setup(associated_col == nullptr)
{
}

nullable_field_base& attrib_initializer_base::get_nfbase_ref() const
{
	return nfbase;
}

//TODO: when you enable using this class directly you would have to create more overloads for this
//		function and/or parse string value to int/float/etc.. and set the nfbase value as well
attrib_initializer_base& attrib_initializer_base::with_default(const std::string& value){
	if(!skip_col_setup){
		attrib_col_ptr->set_default_value(value);
	}
	return *this;
}

attrib_initializer_base& attrib_initializer_base::as_primary(){
	if(!skip_col_setup){
		attrib_col_ptr->set_primary(true);
	}
	return *this;
}

//attrib_initializer_base& as_index();//change column_defs, does it apply on numerical types only?

attrib_initializer_base& attrib_initializer_base::as_nullable(bool value){
	if(!skip_col_setup){
		attrib_col_ptr->set_nullable(true);
	}
	return *this;
}

attrib_initializer_base& attrib_initializer_base::auto_increment(){
	//TODO: check the type is numeric here too
	if(!skip_col_setup){
		attrib_col_ptr->set_auto_increment(true);
	}
	return *this;
}

attrib_initializer_base::~attrib_initializer_base()
{
}

} /* namespace ormplusplus */
