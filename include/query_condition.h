#ifndef INCLUDE_QUERY_CONDITION_H_
#define INCLUDE_QUERY_CONDITION_H_

#include <string>

#include "nullable_field.h"

namespace ormplusplus{

class query_condition{
private:
	std::string col_name;
	std::string _operator;
	nullable_field_base _value;
//	QueryCondition(std::string columnName, std::string _operator);
public:
	query_condition(const std::string& _col_name, const std::string& _operator, const char* _value);
	query_condition(const std::string& _col_name, const std::string& _operator, std::string _value);
	query_condition(const std::string& _col_name, const std::string& _operator, int _value);
	query_condition(const std::string& _col_name, const std::string& _operator, long _value);
	query_condition(const std::string& _col_name, const std::string& _operator, Null _value);

	std::string get_col_name() const;
	std::string get_operator() const;
	std::string get_value_string() const;
	query_condition& operator=(const query_condition& that);
};

}

#endif /* INCLUDE_QUERY_CONDITION_H_ */
