#include "DBSessionBase.h"

#include <sstream>

using namespace std;
namespace ORMPlusPlus {

DBSessionBase::DBSessionBase() {
	// TODO Auto-generated constructor stub
}

void DBSessionBase::dropTable(const std::string& name){
	string queryString = "DROP TABLE IF EXISTS `" + name + "`;";
	execute(queryString);
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
