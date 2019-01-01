#include "QueryCondition.h"

using namespace std;

namespace ORMPlusPlus{

QueryCondition::QueryCondition(string columnName, string operator_, string value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create<string>(value))
{
}

QueryCondition::QueryCondition(string columnName, string operator_, int value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create<int>(value))
{
}

QueryCondition::QueryCondition(string columnName, string operator_, Null value)
: columnName(columnName), operator_(operator_), value(NullableFieldBase::create<nullptr_t>(nullptr))
{
}

string QueryCondition::getColumnName(){ return columnName; };
string QueryCondition::getOperator(){ return operator_; };
string QueryCondition::getValueString(){ return value.toString(); };

}
