#include "db_session_base.h"

#include <sstream>

using namespace std;
namespace ormplusplus {

void db_session_base::drop_table(const std::string& name){
	string query_string = "DROP TABLE IF EXISTS `" + name + "`;";
	execute_void(query_string);
}

bool db_session_base::table_exists(const std::string& name, table_schema& schema){
	auto found_schema = get_table_schema(name);
	for(auto& column : schema){
		table_column& found_column = found_schema[column.first];
		if(!column.second.equals(found_column, column.second.get_precision().is_null())){
			return false;
		}
	}
	return true;
}

std::string db_session_base::build_query_string(const query_base& query){
	std::stringstream query_stream;
	const std::vector<query_condition>& conditions = query.get_conditions_ref();
	const std::vector<order_rule>& order_rules = query.get_order_rules_ref();
	int limit = query.get_limit();

	//TODO: use logging for raw queries
	if(query.get_type() == query_type::_null){
		throw std::runtime_error("try to execute a query with null type");
	}else if(query.get_type() == query_type::_select_single || query.get_type() == query_type::_select_many){
		query_stream << "select ";
		print_col_names(query_stream, query.get_schema_ref());
		query_stream << " from " << query.get_table_name();
		if(!conditions.empty()){
			query_stream << " WHERE ";
		}
		//TODO: nested conditions ?
		for(auto it = conditions.begin(); it != conditions.end(); ++it){
			query_stream << it->get_col_name() << " "
					<< it->get_operator() << " "
					<< it->get_value_string() << " ";
			if(std::next(it) == conditions.end()){
				query_stream << " ";
			}else{
				query_stream << " AND ";
			}
		}

		if(limit != 0){
			query_stream << " limit " << limit;
		}

		if(!order_rules.empty()){
			query_stream << " order by ";
		}
		for(auto it = order_rules.begin(); it != order_rules.end(); ++it){
			query_stream << " " << it->column;
			if(std::next(it) == order_rules.end()){
				query_stream << " ";
			}else{
				query_stream << ", ";
			}
		}

		query_stream<<";";

		return query_stream.str();

	}else if(query.get_type() == query_type::_insert){
		throw std::runtime_error("unimplemented");
	}else if(query.get_type() == query_type::_delete){
		throw std::runtime_error("unimplemented");
	}else if(query.get_type() == query_type::_update){
		throw std::runtime_error("unimplemented");
	}
	throw std::runtime_error("unsupported query_type");
}

db_session_base::~db_session_base() {
	// TODO Auto-generated destructor stub
}

void db_session_base::print_col_names(std::ostream& stream, const table_schema& schema){
	for(auto column_it = schema.begin(); column_it != schema.end(); ++column_it){
		stream << " `" << column_it->first << "`";
		if(std::next(column_it) != schema.end()){
			stream << ",";
		}
	}
}

void db_session_base::print_attrib_values(std::ostream& stream, const table_schema& schema, const attribs_map& attribs){
	for(auto column_it = schema.begin(); column_it != schema.end(); ++column_it){
		//TODO: override operatpr << for nullable field base
		if(!column_it->second.is_integral()){
			stream << " '" << attribs.at(column_it->first).to_string() << "'";
		}else{
			stream << " " << attribs.at(column_it->first).to_string();
		}
		if(std::next(column_it) != schema.end()){
			stream << ",";
		}
	}
}

} /* namespace ormplusplus */
