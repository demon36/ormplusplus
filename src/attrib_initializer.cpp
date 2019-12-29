#include "attrib_initializer.h"
#include "model_base.h"

using namespace std;

namespace ormplusplus{

attrib_initializer::attrib_initializer(const std::string& col_name, table_schema& _schema_ref, model_base& _model_base_ref)
: attrib_col(new table_column(col_name)), schema_ref(_schema_ref), model_base_ref(_model_base_ref), skip_col_setup(_schema_ref.find(col_name) != _schema_ref.end())
{
}

//todo: get rid of this copy operation
attrib_initializer::attrib_initializer(const attrib_initializer& that)
: attrib_col(new table_column(*that.attrib_col)), schema_ref(that.schema_ref), model_base_ref(that.model_base_ref), skip_col_setup(that.skip_col_setup)
{
}

attrib_initializer::attrib_initializer(attrib_initializer&& that)
: attrib_col(that.attrib_col), schema_ref(that.schema_ref), model_base_ref(that.model_base_ref), skip_col_setup(that.skip_col_setup)
{
	that.attrib_col = nullptr;
}

attrib_initializer& attrib_initializer::with_default(const string& value) {
	//TODO: check operator= is implemented correctly
	//this should be set even if skip_col_setup is set because it affects object creation
	attrib_col->set_default_value(value);
	return *this;
}

attrib_initializer& attrib_initializer::auto_increment() {
	//TODO: check the type is numeric
	if(!skip_col_setup){
		attrib_col->set_auto_increment(true);
	}
	return *this;
}

attrib_initializer& attrib_initializer::as_primary() {
	if(!skip_col_setup){
		attrib_col->set_primary(true);
	}
	return *this;
}

attrib_initializer& attrib_initializer::as_nullable(bool value) {
	if(!skip_col_setup){
		attrib_col->set_nullable(value);
	}
	return *this;
}

bool attrib_initializer::has_default_value(){
	return attrib_col->has_default_value();
}

opt_string attrib_initializer::get_default_value(){
	return {attrib_col->get_default_value().get_value_ref(), attrib_col->get_default_value().is_null()};
}

void attrib_initializer::initialize_field(const type_info& type, void* primitive_value_ptr, bool* is_null){
//	static bool add_col_if_not_exists(const std::string& name, const type_info& type)
	attrib_col->set_type_info(type);
	if(schema_ref.find(attrib_col->get_name()) == schema_ref.end()){
		schema_ref.emplace(attrib_col->get_name(), *attrib_col);
	}

	model_base_ref.get_attribs().emplace(
			attrib_col->get_name(), nullable_field_handle(type, primitive_value_ptr, is_null)
		);
}

attrib_initializer::~attrib_initializer(){
	if(attrib_col != nullptr) {
		delete attrib_col;
	}
}

}

