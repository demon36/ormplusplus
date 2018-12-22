#include "DB.h"
#include "QueryBase.h"

namespace ORMPlusPlus {

QueryBase::QueryBase(std::string tableName)
: m_tableName(tableName), whereClause("")
{
}

QueryBase::QueryBase(const QueryBase& queryBase)
: m_tableName(queryBase.m_tableName), whereClause(queryBase.whereClause.str())
{}

void QueryBase::where(std::vector<QueryCondition>& conditions){
	whereClause << " WHERE ";
	for(int i = 0; i < conditions.size(); i++){
		whereClause << conditions[i].getColumnName() << " "
				<< conditions[i].getOperator() << " "
				<< conditions[i].getValue()->toString() << " ";
		if(i < conditions.size() - 1){
			whereClause << " AND ";
		}else{
			whereClause << ";";
		}
	}
}

QueryBase::~QueryBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace ORMPlusPlus */
