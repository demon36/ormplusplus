#ifndef INCLUDE_QUERY_BASE_H_
#define INCLUDE_QUERY_BASE_H_

#include <sstream>
#include <string>
#include <vector>

#include "query_condition.h"

//TODO: use override where possible
namespace ormplusplus {

//use enum classes
enum class query_type{
	_null,
	_select,
	_update,
	_insert,
	_delete,
};

enum class order_type{
	asc,
	desc,
};

struct order_rule{
	const std::string column;
	const order_type type;
};

class query_base {
protected:
	const std::string table_name;
	const table_schema& schema;//used to assert query against
	query_type type = query_type::_null;
	std::vector<query_condition> conditions;
	int limit = 0;
	std::vector<order_rule> order_rules;

public:
	//TODO: add const wherever it is possible
	query_base() = delete;
	query_base(const std::string& _table_name, const table_schema& _schema);
	query_base(const query_base& that) = default;
	const std::string& get_table_name() const;
	const table_schema& get_schema_ref() const;
	query_type get_type() const;
	const std::vector<query_condition>& get_conditions_ref() const;
	int get_limit() const;
	const std::vector<order_rule>& get_order_rules_ref() const;
	virtual ~query_base();
};

} /* namespace ormplusplus */

#endif /* INCLUDE_QUERY_BASE_H_ */
