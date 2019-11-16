#ifndef INCLUDE_DBSESSIONBASE_H_
#define INCLUDE_DBSESSIONBASE_H_

#include "TableColumn.h"
#include "QueryBase.h"
#include "ModelBase.h"
#include "ResultTable.h"

namespace ORMPlusPlus {

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

	/**
	 * this function assumes query has already been checked against model schema
	 */
	template<class UserModel>
	std::vector<UserModel> execute(const QueryBase& query){
		std::vector<UserModel> results;

		ResultTable result = executeFlat(query);
		for(size_t i = 0; i < result.getNumRows(); i++){
			UserModel obj;
			//TODO: no getter here ?
			//TODO: are result columns guaranteed to have same attributes order?
			//todo: optimize the extra copying
			std::map<std::string, NullableFieldBase>& objAttribs = obj.attributes;
			for(auto& attribElement : objAttribs){
				attribElement.second = result.getFieldValue(i, attribElement.first);
			}
			results.emplace_back(obj.clone());
		}
		return results;
	}

	virtual ResultTable executeFlat(const QueryBase& query) = 0;

	virtual ResultTable executeFlat(const std::string& queryString) = 0;

	/**
	 * @return # rows affected
	 */
	virtual std::size_t executeVoid(const std::string& queryString) = 0;
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
