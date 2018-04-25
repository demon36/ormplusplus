#ifndef INCLUDE_QUERYCONDITION_H_
#define INCLUDE_QUERYCONDITION_H_

#include <string>

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
};

}



#endif /* INCLUDE_QUERYCONDITION_H_ */
