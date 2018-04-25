#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>

#include "ORMPlusPlus.h"

using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data;

#define MYSQL_DEFAULT_PORT 3306
#define DEFINE_ATTR(DATATYPE, NAME) DATATYPE& NAME = mapToField<DATATYPE>(#NAME)
#define COLUMN(NAME)(#NAME)

namespace ORMPlusPlus{

class DB{
	static unique_ptr<Session> session;
public:
	static void initialize(string host, string database, string user, string password, int port = MYSQL_DEFAULT_PORT);
	static void executeRawQuery(string query);
};

class NullableFieldBase{
};

//used for accessing renameColumn() and getColumnNames()
//without having to specify template class parameters
class ModelBase{
protected:
	//holds attribute name as key
	//NullableFields ptr as value
	map<string, NullableFieldBase*> fields;
public:
	std::vector<string> getColumnNames(){
		std::vector<string> columnNames;
		for(auto& field : fields){
			columnNames.push_back(field.first);
		}
		return columnNames;
	}
};

template<class DerivedType, class PrimitiveType>
class NullableField : public NullableFieldBase{
	ModelBase* modelPtr = nullptr;
	//column name in the database
	string columnName = "";
	bool isNull = true;
	bool isPrimaryKey = false;
protected:
	PrimitiveType primitiveValue;
	bool requireQuotes = true;
public:
	NullableField();

	//initialize column name with the same attribute name value
	NullableField(ModelBase* OwnerModel, string attributeName){
		this->columnName = attributeName;
		modelPtr = OwnerModel;
	}

	NullableField(PrimitiveType value){
		primitiveValue = value;
	}

	DerivedType& withColumnName(string customColumnName){
		if(modelPtr == nullptr){
			throw runtime_error("NullableField is not bound to model");
		}
		columnName = customColumnName;
		return static_cast<DerivedType&>(*this);
	}

	string getColumnName(){
		return columnName;
	}

	DerivedType& asPrimaryKey(){
		isPrimaryKey = true;
		return static_cast<DerivedType&>(*this);
	}

	DerivedType& setToNull(){
		isNull = true;
		return static_cast<DerivedType&>(*this);
	}

	PrimitiveType& get(){
		return primitiveValue;
	}

	DerivedType& withDefault(PrimitiveType value){
		primitiveValue = value;
		return static_cast<DerivedType&>(*this);
	}
};

class Integer : public NullableField<Integer, int>{
	using NullableField::NullableField;
};

class String : public NullableField<String, string>{
	using NullableField::NullableField;
};

class Condition{
	string columnName;
	string operator_;
	NullableFieldBase* value = nullptr;
	Condition(string columnName, string operator_){
		this->columnName = columnName;
		this->operator_ = operator_;
	}
public:
	Condition(string columnName, string operator_, string value)
	: Condition(columnName, operator_)
	{
		this->value = new String(value);
	}

	Condition(string columnName, string operator_, int value)
	: Condition(columnName, operator_)
	{
		this->value = new Integer(value);
	}
};

template<class UserModel>
class Query{
	string whereClause = "WHERE ";
public:
	Query(){};
	Query(vector<Condition> conditions){
		for(auto& condition : conditions){
		}
	}

	std::vector<UserModel> get(){

	}

	void remove();
	Query& update(std::vector<string>);
	void set(std::vector<string>);
	long long count();
	long long sum();
};

template<class UserModel>
class Model : public ModelBase
{
public:
	virtual string getTableName() = 0;
	virtual ~Model(){};

	static std::vector<UserModel> get(){
		Query<UserModel> query;
		return query.get();
	}

	static Query<UserModel> where(std::vector<Condition> conditions){
		Query<UserModel> query;
		return query;
	}

	template<typename FieldType>
	FieldType& mapToField(string attributeName){
		fields[attributeName] = new FieldType(this, attributeName);
		return static_cast<FieldType&>(*fields[attributeName]);
	}

};
}

#endif MODEL_H
