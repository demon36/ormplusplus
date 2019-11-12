#ifndef INCLUDE_DBSESSIONBASE_H_
#define INCLUDE_DBSESSIONBASE_H_

#include <memory>
#include <list>

#include "TableColumn.h"
#include "QueryBase.h"
#include "ModelBase.h"

namespace ORMPlusPlus {

//todo: implement optimized class ResultTable
typedef std::list<std::map<std::string, std::string>> ResultTable;

class DBSessionBase {
public:
	DBSessionBase();
	virtual void createTable(const std::string& name, const TableSchema& schema) = 0;
	void dropTable(const std::string& name);
	virtual bool tableExists(const std::string& name) = 0;
	bool tableExists(const std::string& name, TableSchema& schema);
	virtual TableSchema getTableSchema(const std::string& name) = 0;
	virtual std::string buildQueryString(const QueryBase& query);
	/**
	 * inserts the provided model or updates it if exists
	 */
//	virtual void save(const ModelBase& model) = 0;
	//non const ModelBase ref since auto increment pkey might be altered
	/**
	 * insert a new row for provided model
	 * @param updateAutoIncPKey whether or not make additional query
	 * for extracting inserted row id and updating provided model
	 */
	virtual void insert(ModelBase& model, bool updateAutoIncPKey = false) = 0;
//	virtual void update(const ModelBase& model) = 0;

	//TODO: find a soln for the following issue:
	//		this function should be marked as virtual in order to get overridden
	//		by different database sessions, but c++ prohibits virtual template fns
	//TODO: split this fn
	/**
	 * this function assumes query has already been checked against model schema
	 */

	template<class UserModel>
	std::vector<UserModel> execute(const QueryBase& query){
		std::vector<UserModel> results;

		ResultTable result = executeFlat(query);
		for(auto row = result.begin(); row != result.end(); ++row){
			UserModel obj;
			//TODO: no getter here ?
			//TODO: are result columns guaranteed to have same attributes order?
			std::map<std::string, NullableFieldBase>& objAttribs = obj.attributes;
			for(auto& attribElement : objAttribs){
				const std::string& attribName = attribElement.first;
				NullableFieldBase& attrib = attribElement.second;
				const std::type_info& attribPrimitiveType = attrib.getType();

				//TODO: change this later
				if(attribPrimitiveType == typeid(int)){
					attrib = stoi(row->at(attribName));// (*it)[attribName].convert<int>();
				}else if(attribPrimitiveType == typeid(long)){
					attrib = stol(row->at(attribName));;
				}else if(attribPrimitiveType == typeid(float)){
					attrib = stof(row->at(attribName));
				}else if(attribPrimitiveType == typeid(double)){
					attrib = stod(row->at(attribName));
				}else if(attribPrimitiveType == typeid(std::string)){
					attrib = row->at(attribName);
				}else if(attribPrimitiveType == typeid(::tm)){
					//TODO: query datetime with same format
					//TODO: use correct tz
					strptime(row->at(attribName).c_str(), "%Y-%m-%dT%H:%M:%SZ", &attrib.getValueRef<::tm>());
				}else if(attribPrimitiveType == typeid(nullptr_t)){
					//TODO get rid of this case
				}
			}
			results.emplace_back(obj.clone());
		}
		return results;
	}

	//TODO: use proper names
	virtual ResultTable executeFlat(const QueryBase& query) = 0;

	virtual ResultTable executeRawQuery(const std::string& queryString) = 0;

	/**
	 * @return # rows affected
	 */
	virtual std::size_t executeNonQuery(const std::string& queryString) = 0;
	virtual ~DBSessionBase();
protected:
	//TODO: move to modelBase OR create class schema instead of typedef and override << for it
	/**
	 * prints comma separated schema map keys ex: "`c1`, `c2` ,..." to specified stream
	 */
	static void printColumnNames(std::ostream& stream, const TableSchema& schema);

	/**
	 * prints comma separated model attribute values ex: "v1,v2,..." to specified stream<br>
	 * schema is needed for type checking
	 */
	//TODO: commas needed
	static void printAttribValues(std::ostream& stream, const TableSchema& schema, const AttributesMap& attribs);
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_DBSESSIONBASE_H_ */
