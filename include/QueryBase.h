#ifndef INCLUDE_QUERYBASE_H_
#define INCLUDE_QUERYBASE_H_

#include <sstream>
#include <string>
#include <vector>

#include "QueryCondition.h"

namespace ORMPlusPlus {

class QueryBase {
	const std::string m_tableName;
	std::stringstream whereClause;
public:
	QueryBase(std::string tableName);
	QueryBase(const QueryBase& queryBase);
	void where(std::vector<QueryCondition>& conditions);
	virtual ~QueryBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_QUERYBASE_H_ */
