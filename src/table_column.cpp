#include "table_column.h"

#include <type_info.h>
#include "nullable_field_base.h"


#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 10

using namespace std;

namespace ormplusplus{

table_column::table_column()
: type(Null::get_type_info())//TODO: check the consequences
{}

table_column::table_column(const string& _name, const type_info& _typeInfo, long _length, long _precision, bool _nullable, bool _pKey, bool _auto_increment)
: name(_name), type(_typeInfo)
{
	this->length = _length;
	this->precision = _precision;
	this->nullable = _nullable;
	if(_nullable){//if column is nullable then default value is NULL
		default_value_set = true;
	}
	this->is_pkey = _pKey;
	this->is_auto_inc = _auto_increment;
}

table_column::table_column(const string& _name, const type_info& _typeInfo)
: name(_name), type(_typeInfo)
{
	if(is_integral()){
		length = -1;
		precision = DEFAULT_NUM_PRECISION;
	}else if(is_text()){
		length = DEFAULT_STRING_LENGTH;
		precision = -1;
	}
}

string table_column::get_name() const { return name; }
const type_info& table_column::get_type_info() const { return type; }
long table_column::get_length() const { return length; }
long table_column::get_precision() const { return precision; }
bool table_column::is_nullable() const { return nullable; }
bool table_column::has_default_value() const { return default_value_set; }
String table_column::get_default_value() const { return default_value; }
bool table_column::is_auto_increment() const { return is_auto_inc; }
bool table_column::is_primary_key() const { return is_pkey; }

void table_column::set_length(int value){
	if(!is_text()){
		throw runtime_error("trying to set length on non-text type");
	}
	length = value;
}

void table_column::set_precision(int value){
	if(!is_text()){
		throw runtime_error("trying to set precision on non-numeric type");
	}
	length = value;
}

//TODO: should this be setAsIndex ??
void table_column::set_primary(bool value){
	is_pkey = value;
}

void table_column::set_nullable(bool value){
	nullable = value;
	if(nullable && !default_value_set){
		default_value_set = true;
	}
	//todo: this probably needs a revisit, notice this scenario:
	//use constructs TableColumn with nullable = true
	//defaultValueSet gets set to true because NULL becomes the default value
	//user calls setNullable(false)
	//now we need to clear defaultValueSet
	//but what if user already sat defaultValue to something different that NULL ?
	//todo: change defaultValue representation to be compatible with database field
	//possible values: "NULL", "34", "'a string value'", "'NULL'"
}

void table_column::set_default_value(const String& value){
	default_value = value;
	default_value_set = true;
}

void table_column::set_auto_increment(bool value){
	is_auto_inc = value;
}

bool table_column::is_integral() const {
	return type.is_integral;
}

bool table_column::is_text() const {
	return type.is_text;
}

bool table_column::operator==(const table_column& that){
	//TODO: update with added attributes
	if(
		this->name == that.name &&
		this->type == that.type &&
		this->nullable == that.nullable &&
		this->default_value == that.default_value &&
		this->is_pkey == that.is_pkey &&
		this->is_auto_inc == that.is_auto_inc &&
		this->length == that.length &&
		this->precision == that.precision
	){
		return true;
	}else{
		return false;
	}
}

bool table_column::operator!=(const table_column& that){
	return !operator==(that);
}

}
