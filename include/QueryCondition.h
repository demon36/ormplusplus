#ifndef INCLUDE_QUERYCONDITION_H_
#define INCLUDE_QUERYCONDITION_H_

#include <string>

#include "NullableField.h"

namespace ORMPlusPlus{

class QueryCondition{
private:
	std::string columnName;
	std::string operator_;
	NullableFieldBase value;
//	QueryCondition(std::string columnName, std::string operator_);
public:
	QueryCondition(std::string columnName, std::string operator_, const char* value);
	QueryCondition(std::string columnName, std::string operator_, std::string value);
	QueryCondition(std::string columnName, std::string operator_, int value);
	QueryCondition(std::string columnName, std::string operator_, Null value);

	std::string getColumnName();
	std::string getOperator();
	std::string getValueString();
};

}



#endif /* INCLUDE_QUERYCONDITION_H_ */
