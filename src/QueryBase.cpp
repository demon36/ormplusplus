#include "DB.h"
#include "QueryBase.h"

namespace ORMPlusPlus {

QueryBase::QueryBase(const std::string& _tableName, const TableSchema& _schema)
: tableName(_tableName), schema(_schema)
{
}

const std::string& QueryBase::getTableName() const{
	return tableName;
}

const TableSchema& QueryBase::getTableSchema() const{
	return schema;
}

QueryType QueryBase::getType() const{
	return type;
}

const std::vector<QueryCondition>& QueryBase::getConditionsRef() const{
	return conditions;
}

int QueryBase::getLimit() const{
	return limit;
}

const std::vector<OrderRule>& QueryBase::getOrderRulesRef() const{
	return orderRules;
}

QueryBase::~QueryBase(){
}

} /* namespace ORMPlusPlus */
