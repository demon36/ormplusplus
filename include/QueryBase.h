#ifndef INCLUDE_QUERYBASE_H_
#define INCLUDE_QUERYBASE_H_

#include <sstream>
#include <string>
#include <vector>

#include "QueryCondition.h"

//TODO: use override where possible
namespace ORMPlusPlus {

//use enum classes
enum class QueryType{
	_Null,
	_Select,
	_Update,
	_Insert,
	_Delete,
};

enum class OrderType{
	Asc,
	Desc,
};

struct OrderRule{
	const std::string column;
	const OrderType type;
};

class QueryBase {
protected:
	const std::string tableName;
	const TableSchema& schema;//used to assert query against
	QueryType type = QueryType::_Null;
	std::vector<QueryCondition> conditions;
	int limit = 0;
	std::vector<OrderRule> orderRules;

public:
	//TODO: add const wherever it is possible
	QueryBase() = delete;
	QueryBase(const std::string& _tableName, const TableSchema& _schema);
	QueryBase(const QueryBase& queryBase) = default;
	const std::string& getTableName() const;
	const TableSchema& getTableSchema() const;
	QueryType getType() const;
	const std::vector<QueryCondition>& getConditionsRef() const;
	int getLimit() const;
	const std::vector<OrderRule>& getOrderRulesRef() const;
	virtual ~QueryBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_QUERYBASE_H_ */
