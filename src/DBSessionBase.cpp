#include "DBSessionBase.h"

#include <sstream>

using namespace std;
namespace ORMPlusPlus {

DBSessionBase::DBSessionBase() {
	// TODO Auto-generated constructor stub
}

void DBSessionBase::createTable(const string& name, const TableSchema& schema){
	stringstream queryStream;
	queryStream << "CREATE TABLE IF NOT EXISTS `"<< name <<"`(";
	vector<TableColumn> columnsList;
	for(auto& columnEntry : schema ) {
		columnsList.push_back( columnEntry.second );
	}

	string primaryKeys;
	for(int i = 0; i < columnsList.size(); i++){
		if(columnsList[i].isPrimary()){
			if(!primaryKeys.empty()){
				primaryKeys += ",";
			}
			primaryKeys += columnsList[i].getName();
		}
		queryStream << "`"<< columnsList[i].getName() <<"` " <<  columnsList[i].getDBTypeName();
		if(columnsList[i].isText()){
			queryStream << "(" << columnsList[i].getLength() << ")";
		}
		if(i < columnsList.size()-1){
			queryStream << ", ";
		}
	}

	if(!primaryKeys.empty()){
		queryStream << " PRIMARY KEY (%s) ", primaryKeys;
	}
	queryStream << ");";

	execute(queryStream.str());
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

} /* namespace ORMPlusPlus */
