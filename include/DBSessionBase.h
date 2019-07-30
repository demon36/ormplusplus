#ifndef INCLUDE_DBSESSIONBASE_H_
#define INCLUDE_DBSESSIONBASE_H_

#include <memory>

//TODO::remove dependency on Poco here
#include <Poco/Data/RecordSet.h>

#include "TableColumn.h"
#include "QueryBase.h"
#include "ModelBase.h"

namespace ORMPlusPlus {

class DBSessionBase {
public:
	DBSessionBase();
	virtual void createTable(const std::string& name, const TableSchema& schema) = 0;
	void dropTable(const std::string& name);
	virtual bool tableExists(const std::string& name) = 0;
	bool tableExists(const std::string& name, TableSchema& schema);
	virtual TableSchema getTableSchema(const std::string& name) = 0;

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
		std::stringstream queryStream;
		std::string tableName = UserModel::getTableName();
		const std::vector<QueryCondition>& conditions = query.getConditionsRef();
		const std::vector<OrderRule>& orderRules = query.getOrderRulesRef();
		int limit = query.getLimit();
		std::vector<UserModel> results;

		//TODO: use logging for raw queries
		if(query.getType() == QueryType::_Null){
			throw std::runtime_error("try to execute a query with null type");
		}else if(query.getType() == QueryType::_Select){
			queryStream << "select ";
			printColumnNames(queryStream, UserModel::getSchema());
			queryStream << " from " << tableName;
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

			Poco::Data::RecordSet result = execute(queryStream.str());
			for(auto it = result.begin(); it != result.end(); ++it){
				UserModel obj;
				//TODO: no getter here ?
				std::map<std::string, NullableFieldBase>& objAttribs = obj.attributes;
				for(auto& attribElement : objAttribs){
					const std::string& attribName = attribElement.first;
					NullableFieldBase& attrib = attribElement.second;
					const std::type_info& attribPrimitiveType = attrib.getType();

					//TODO: change this later
					if(attribPrimitiveType == typeid(int)){
						attrib = (*it)[attribName].convert<int>();
					}else if(attribPrimitiveType == typeid(long)){
						attrib = (*it)[attribName].convert<long>();
					}else if(attribPrimitiveType == typeid(float)){
						attrib = (*it)[attribName].convert<float>();
					}else if(attribPrimitiveType == typeid(double)){
						attrib = (*it)[attribName].convert<double>();
					}else if(attribPrimitiveType == typeid(std::string)){
						attrib = (*it)[attribName].convert<std::string>();
					}else if(attribPrimitiveType == typeid(Poco::DateTime)){
						attrib = (*it)[attribName].convert<Poco::DateTime>();
					}else if(attribPrimitiveType == typeid(nullptr_t)){
						//TODO get rid of this case
					}
				}
				results.emplace_back(obj.clone());
			}
			return results;

		}if(query.getType() == QueryType::_Insert){
			throw std::runtime_error("unimplemented");
		}if(query.getType() == QueryType::_Delete){
			throw std::runtime_error("unimplemented");
		}if(query.getType() == QueryType::_Update){
			throw std::runtime_error("unimplemented");
		}
		throw std::runtime_error("unsupported QueryType");
	}

	virtual Poco::Data::RecordSet execute(const std::string& query) = 0;

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
