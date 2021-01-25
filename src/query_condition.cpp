#include "query_condition.h"

using namespace std;

namespace ormplusplus{

query_condition::query_condition(const string& _column_name, const string& _operator, string _value)
: col_name(_column_name), operator_(_operator), value(_value)//value_handle(nullable_field_handle::create(db_string::get_type_info(), _value))
{
}

query_condition::query_condition(const string& _column_name, const string& _operator, int32_t _value)
: col_name(_column_name), operator_(_operator), value(to_string(_value))//value_handle(nullable_field_handle::create(db_int::get_type_info(), _value))
{
}

query_condition::query_condition(const string& _column_name, const string& _operator, int64_t _value)
: col_name(_column_name), operator_(_operator), value(to_string(_value))//value_handle(nullable_field_handle::create(db_long::get_type_info(), _value))
{
}

query_condition::query_condition(const string& _column_name, const string& _operator, db_null _value)
: col_name(_column_name), operator_(_operator), value()//value_handle(nullable_field_handle::create(db_null::get_type_info(), nullptr))
{
}

string query_condition::get_col_name() const { return col_name; };
string query_condition::get_operator() const { return operator_; };
string query_condition::get_value_string() const { return value; };

query_condition& query_condition::operator=(const query_condition& that){
	this->col_name = that.col_name;
	this->operator_ = that.operator_;
	this->value = that.value;
	return *this;
}

}
