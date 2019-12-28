#include "attrib_initializer.h"

using namespace std;

namespace ormplusplus{

attrib_initializer::attrib_initializer(const std::string& col_name, table_schema& _schema_ref, model_base& _model_base_ref)
: attrib_col(col_name), schema_ref(_schema_ref), model_base_ref(_model_base_ref), skip_col_setup(_schema_ref.find(col_name) != _schema_ref.end())
{
}

attrib_initializer& attrib_initializer::with_default(const string& value) {
	//TODO: check operator= is implemented correctly
	if(!skip_col_setup){
		attrib_col.set_default_value(value, false);
	}
	return *this;
}

attrib_initializer& attrib_initializer::auto_increment() {
	//TODO: check the type is numeric
	if(!skip_col_setup){
		attrib_col.set_auto_increment(true);
	}
	return *this;
}

attrib_initializer& attrib_initializer::as_primary() {
	if(!skip_col_setup){
		attrib_col.set_primary(true);
	}
	return *this;
}

attrib_initializer& attrib_initializer::as_nullable(bool value) {
	if(!skip_col_setup){
		attrib_col.set_nullable(value);
	}
	return *this;
}

attrib_initializer::~attrib_initializer(){
}

}

