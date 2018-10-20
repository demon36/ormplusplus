#ifndef INCLUDE_QUERYCONDITION_H_
#define INCLUDE_QUERYCONDITION_H_

#include <string>

#include "NullableField.h"

using namespace std;

namespace ORMPlusPlus{

class QueryCondition{
	string columnName;
	string operator_;
	NullableFieldBase* value = nullptr;
	QueryCondition(string columnName, string operator_){
		this->columnName = columnName;
		this->operator_ = operator_;
	}
public:
	string getColumnName(){ return columnName; };
	string getOperator(){ return operator_; };
	NullableFieldBase* getValue(){ return value; };
	QueryCondition(string columnName, string operator_, string value)
	: QueryCondition(columnName, operator_)
	{
		this->value = new String(value);
	}

	QueryCondition(string columnName, string operator_, int value)
	: QueryCondition(columnName, operator_)
	{
		this->value = new Integer(value);
	}

	QueryCondition(string columnName, string operator_, NullableFieldBase value)
	: QueryCondition(columnName, operator_)
	{
		this->value = new NullableFieldBase;
	}
};

}



#endif /* INCLUDE_QUERYCONDITION_H_ */
