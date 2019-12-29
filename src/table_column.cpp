#include "table_column.h"

#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 12

using namespace std;

namespace ormplusplus{

table_column::table_column()
{
}

table_column::table_column(const string& _name)
: name(_name), type(&db_null::get_type_info())
{
}

table_column::table_column(const string& _name, const type_info& _type_info, db_long _length, db_long _precision, bool _nullable, bool _pkey, bool _auto_increment)
: name(_name), type(&_type_info)
{
	this->length = _length;
	this->precision = _precision;
	this->nullable = _nullable;
	if(_nullable){//if column is nullable then default value is NULL
		default_value_set = true;
	}
	this->is_pkey = _pkey;
	this->is_auto_inc = _auto_increment;
}

string table_column::get_name() const { return name; }
const type_info& table_column::get_type_info() const { return *type; }
db_long table_column::get_length() const { return length; }
db_long table_column::get_precision() const { return precision; }
bool table_column::is_nullable() const { return nullable; }
bool table_column::has_default_value() const { return default_value_set; }
db_string table_column::get_default_value () const { return default_value; }
bool table_column::is_auto_increment() const { return is_auto_inc; }
bool table_column::is_primary_key() const { return is_pkey; }

void table_column::set_type_info(const type_info& _type){
	type = &_type;
	if(type->is_text && length.is_null()){
		length = DEFAULT_STRING_LENGTH;
		precision.clear_value();
	}
}

void table_column::set_length(const db_long& nullable_value){
	if(!is_text()){
		throw runtime_error("trying to set length on non-text type");
	}
	length = nullable_value;
}

void table_column::set_precision(const db_long& nullable_value){
	if(!is_text()){
		throw runtime_error("trying to set precision on non-numeric type");
	}
	length = nullable_value;
}

//TODO: should this be set_as_index ??
void table_column::set_primary(bool value){
	is_pkey = value;
}

void table_column::set_nullable(bool value){
	nullable = value;
	if(nullable && !default_value_set){
		default_value_set = true;
	}
	//todo: this probably needs a revisit, notice this scenario:
	//use constructs table_column with nullable = true
	//default_value_set gets set to true because NULL becomes the default value
	//user calls set_nullable(false)
	//now we need to clear default_value_set
	//but what if user already sat default_value to something different that NULL ?
	//todo: change default_value representation to be compatible with database field
	//possible values: "NULL", "34", "'a string value'", "'NULL'"
}

void table_column::set_default_value(const db_string& nullable_value){
	default_value = nullable_value;
	default_value_set = true;
}

void table_column::set_auto_increment(bool value){
	is_auto_inc = value;
}

bool table_column::is_integral() const {
	return type->is_integral;
}

bool table_column::is_text() const {
	return type->is_text;
}

bool table_column::equals(const table_column& that, bool ignore_precision){
	//TODO: update with added attributes
	if(
		this->name == that.name &&
		*this->type == *that.type &&
		this->nullable == that.nullable &&
		this->default_value == that.default_value &&
		this->is_pkey == that.is_pkey &&
		this->is_auto_inc == that.is_auto_inc &&
		this->length == that.length &&
		(ignore_precision || this->precision == that.precision)
	){
		return true;
	}else{
		return false;
	}
}

}
