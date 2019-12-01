#include "QueryCondition.h"

using namespace std;

namespace ORMPlusPlus{

QueryCondition::QueryCondition(const string& columnName, const string& operator_, string value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create(String::getTypeInfo(), value))
{
}

QueryCondition::QueryCondition(const string& columnName, const string& operator_, int value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create(Integer::getTypeInfo(), value))
{
}

QueryCondition::QueryCondition(const string& columnName, const string& operator_, long value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create(Long::getTypeInfo(), value))
{
}

QueryCondition::QueryCondition(const string& columnName, const string& operator_, Null value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create(Null::getTypeInfo(), nullptr))
{
}

string QueryCondition::getColumnName() const { return columnName; };
string QueryCondition::getOperator() const { return operator_; };
string QueryCondition::getValueString() const { return value.toString(); };

QueryCondition& QueryCondition::operator=(const QueryCondition& that){
	this->columnName = that.columnName;
	this->operator_ = that.operator_;
	this->value = that.value;
	return *this;
}

}
