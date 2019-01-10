#include "QueryCondition.h"

using namespace std;

namespace ORMPlusPlus{

QueryCondition::QueryCondition(const string& columnName, const string& operator_, string value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create<string>(value))
{
}

QueryCondition::QueryCondition(const string& columnName, const string& operator_, int value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create<int>(value))
{
}

QueryCondition::QueryCondition(const string& columnName, const string& operator_, Null value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create<nullptr_t>(nullptr))
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
