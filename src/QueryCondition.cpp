#include "QueryCondition.h"

using namespace std;

namespace ORMPlusPlus{

QueryCondition::QueryCondition(string columnName, string operator_){
	this->columnName = columnName;
	this->operator_ = operator_;
}

QueryCondition::QueryCondition(string columnName, string operator_, string value)
: QueryCondition(columnName, operator_)
{
	this->value = new String(value);
}

QueryCondition::QueryCondition(string columnName, string operator_, int value)
: QueryCondition(columnName, operator_)
{
	this->value = new Integer(value);
}

QueryCondition::QueryCondition(string columnName, string operator_, NullableFieldBase value)
: QueryCondition(columnName, operator_)
{
	this->value = new NullableFieldBase;
}

string QueryCondition::getColumnName(){ return columnName; };
string QueryCondition::getOperator(){ return operator_; };
NullableFieldBase* QueryCondition::getValue(){ return value; };

}
