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
	QueryCondition(const std::string& columnName, const std::string& operator_, const char* value);
	QueryCondition(const std::string& columnName, const std::string& operator_, std::string value);
	QueryCondition(const std::string& columnName, const std::string& operator_, int value);
	QueryCondition(const std::string& columnName, const std::string& operator_, long value);
	QueryCondition(const std::string& columnName, const std::string& operator_, Null value);

	std::string getColumnName() const;
	std::string getOperator() const;
	std::string getValueString() const;
	QueryCondition& operator=(const QueryCondition& that);
};

}



#endif /* INCLUDE_QUERYCONDITION_H_ */
