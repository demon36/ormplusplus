#include <db.h>
#include "query_base.h"

namespace ormplusplus {

query_base::query_base(const std::string& _tableName, const table_schema& _schema)
: table_name(_tableName), schema(_schema)
{
}

const std::string& query_base::get_table_name() const{
	return table_name;
}

const table_schema& query_base::get_schema_ref() const{
	return schema;
}

query_type query_base::get_type() const{
	return type;
}

const std::vector<query_condition>& query_base::get_conditions_ref() const{
	return conditions;
}

int query_base::get_limit() const{
	return limit;
}

const std::vector<order_rule>& query_base::get_order_rules_ref() const{
	return order_rules;
}

query_base::~query_base(){
}

} /* namespace ORMPlusPlus */