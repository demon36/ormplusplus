#include "db_session_base.h"

#include <sstream>

using namespace std;
namespace ormplusplus {

db_session_base::db_session_base() {
	// TODO Auto-generated constructor stub
}

void db_session_base::drop_table(const std::string& name){
	string query_string = "DROP TABLE IF EXISTS `" + name + "`;";
	execute_void(query_string);
}

bool db_session_base::table_exists(const std::string& name, table_schema& schema){
	auto foundSchema = get_table_schema(name);
	for(auto& column : schema){
		table_column& foundColumn = foundSchema[column.first];
		if(column.second != foundColumn){
			return false;
		}
		if(column.second.is_integral() && column.second.get_precision() > foundColumn.get_precision()){
			return false;
		}
		//TODO: can it be both ??
		if(column.second.is_text() && column.second.get_length() > foundColumn.get_length()){
			return false;
		}
	}
	return true;
}

std::string db_session_base::build_query_string(const query_base& query){
	std::stringstream queryStream;
	const std::vector<query_condition>& conditions = query.getConditionsRef();
	const std::vector<order_rule>& orderRules = query.getOrderRulesRef();
	int limit = query.getLimit();

	//TODO: use logging for raw queries
	if(query.getType() == query_type::_Null){
		throw std::runtime_error("try to execute a query with null type");
	}else if(query.getType() == query_type::_Select){
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

	}else if(query.getType() == query_type::_Insert){
		throw std::runtime_error("unimplemented");
	}else if(query.getType() == query_type::_Delete){
		throw std::runtime_error("unimplemented");
	}else if(query.getType() == query_type::_Update){
		throw std::runtime_error("unimplemented");
	}
	throw std::runtime_error("unsupported QueryType");
}

db_session_base::~db_session_base() {
	// TODO Auto-generated destructor stub
}

void db_session_base::printColumnNames(std::ostream& stream, const table_schema& schema){
	for(auto columnIt = schema.begin(); columnIt != schema.end(); ++columnIt){
		stream << " `" << columnIt->first << "`";
		if(std::next(columnIt) != schema.end()){
			stream << ",";
		}
	}
}

void db_session_base::printAttribValues(std::ostream& stream, const table_schema& schema, const AttributesMap& attribs){
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
