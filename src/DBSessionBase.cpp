#include "DBSessionBase.h"

#include <sstream>

using namespace std;
namespace ORMPlusPlus {

DBSessionBase::DBSessionBase() {
	// TODO Auto-generated constructor stub
}

void DBSessionBase::dropTable(const std::string& name){
	string queryString = "DROP TABLE IF EXISTS `" + name + "`;";
	executeVoid(queryString);
}

bool DBSessionBase::tableExists(const std::string& name, TableSchema& schema){
	auto foundSchema = getTableSchema(name);
	for(auto& column : schema){
		TableColumn& foundColumn = foundSchema[column.first];
		if(column.second != foundColumn){
			return false;
		}
		if(column.second.isIntegral() && column.second.getPrecision() > foundColumn.getPrecision()){
			return false;
		}
		//TODO: can it be both ??
		if(column.second.isText() && column.second.getLength() > foundColumn.getLength()){
			return false;
		}
	}
	return true;
}

std::string DBSessionBase::buildQueryString(const QueryBase& query){
	std::stringstream queryStream;
	const std::vector<QueryCondition>& conditions = query.getConditionsRef();
	const std::vector<OrderRule>& orderRules = query.getOrderRulesRef();
	int limit = query.getLimit();

	//TODO: use logging for raw queries
	if(query.getType() == QueryType::_Null){
		throw std::runtime_error("try to execute a query with null type");
	}else if(query.getType() == QueryType::_Select){
		queryStream << "select ";
		printColumnNames(queryStream, query.getTableSchema());
		queryStream << " from " << query.getTableName();
		if(!conditions.empty()){
			queryStream << " WHERE ";
		}
		//TODO: nested conditions ?
		for(auto it = conditions.begin(); it != conditions.end(); ++it){
			queryStream << it->getColumnName() << " "
					<< it->getOperator() << " "
					<< it->getValueString() << " ";
			if(std::next(it) == conditions.end()){
				queryStream << " ";
			}else{
				queryStream << " AND ";
			}
		}

		if(limit != 0){
			queryStream << " limit " << limit;
		}

		if(!orderRules.empty()){
			queryStream << " order by ";
		}
		for(auto it = orderRules.begin(); it != orderRules.end(); ++it){
			queryStream << " " << it->column;
			if(std::next(it) == orderRules.end()){
				queryStream << " ";
			}else{
				queryStream << ", ";
			}
		}

		queryStream<<";";

		return queryStream.str();

	}else if(query.getType() == QueryType::_Insert){
		throw std::runtime_error("unimplemented");
	}else if(query.getType() == QueryType::_Delete){
		throw std::runtime_error("unimplemented");
	}else if(query.getType() == QueryType::_Update){
		throw std::runtime_error("unimplemented");
	}
	throw std::runtime_error("unsupported QueryType");
}

DBSessionBase::~DBSessionBase() {
	// TODO Auto-generated destructor stub
}

void DBSessionBase::printColumnNames(std::ostream& stream, const TableSchema& schema){
	for(auto columnIt = schema.begin(); columnIt != schema.end(); ++columnIt){
		stream << " `" << columnIt->first << "`";
		if(std::next(columnIt) != schema.end()){
			stream << ",";
		}
	}
}

void DBSessionBase::printAttribValues(std::ostream& stream, const TableSchema& schema, const AttributesMap& attribs){
	for(auto columnIt = schema.begin(); columnIt != schema.end(); ++columnIt){
		//TODO: override operatpr << for nullable field base
		if(!columnIt->second.isIntegral()){
			stream << " '" << attribs.at(columnIt->first).toString() << "'";
		}else{
			stream << " " << attribs.at(columnIt->first).toString();
		}
		if(std::next(columnIt) != schema.end()){
			stream << ",";
		}
	}
}

} /* namespace ORMPlusPlus */
