#include "query_condition.h"

using namespace std;

namespace ormplusplus{

query_condition::query_condition(const string& _columnName, const string& _operator, string _value)
: col_name(_columnName), operator_(_operator), value(nullable_field_base::create(String::getTypeInfo(), _value))
{
}

query_condition::query_condition(const string& _columnName, const string& _operator, int _value)
: col_name(_columnName), operator_(_operator), value(nullable_field_base::create(Integer::getTypeInfo(), _value))
{
}

query_condition::query_condition(const string& _columnName, const string& _operator, long _value)
: col_name(_columnName), operator_(_operator), value(nullable_field_base::create(Long::getTypeInfo(), _value))
{
}

query_condition::query_condition(const string& _columnName, const string& _operator, Null _value)
: col_name(_columnName), operator_(_operator), value(nullable_field_base::create(Null::getTypeInfo(), nullptr))
{
}

string query_condition::get_col_name() const { return col_name; };
string query_condition::get_operator() const { return operator_; };
string query_condition::get_value_string() const { return value.to_string(); };

query_condition& query_condition::operator=(const query_condition& that){
	this->col_name = that.col_name;
	this->operator_ = that.operator_;
	this->value = that.value;
	return *this;
}

}
