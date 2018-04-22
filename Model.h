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

class nullableBase{
};

//used for accessing renameColumn() and getColumnNames()
//without having to specify template class parameters
class ModelBase{
protected:
	map<string, nullableBase*> fields;
public:
	void renameColumn(string oldColumnName, string newColumnName){
		fields[newColumnName] = fields[oldColumnName];
		fields.erase(fields.find(oldColumnName));
	}

	std::vector<string> getColumnNames(){
		std::vector<string> columnNames;
		for(auto& field : fields){
			columnNames.push_back(field.first);
		}
		return columnNames;
	}
};



template<class DerivedType, class PrimitiveType>
class NullableField : public nullableBase{
	ModelBase* modelPtr = nullptr;
	string columnName = "";
	bool isNull = true;
	bool isPrimaryKey = false;
protected:
	PrimitiveType primitiveValue;
	bool requireQuotes = true;
public:
	NullableField();

	NullableField(ModelBase* OwnerModel, string columnName){
		this->columnName = columnName;
		modelPtr = OwnerModel;
	}

	NullableField(PrimitiveType value){
		primitiveValue = value;
	}

	//changes already set column name
	DerivedType& withColumnName(string newColumnName){
		if(modelPtr == nullptr){
			throw runtime_error("NullableField is not bound to model");
		}
		modelPtr->renameColumn(columnName, newColumnName);
		columnName = newColumnName;
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

class condition{
	string columnName;
	string operator_;
	nullableBase* value = nullptr;
	condition(string columnName, string operator_){
		this->columnName = columnName;
		this->operator_ = operator_;
	}
public:
	condition(string columnName, string operator_, string value)
	: condition(columnName, operator_)
	{
		this->value = new String(value);
	}

	condition(string columnName, string operator_, int value)
	: condition(columnName, operator_)
	{
		this->value = new Integer(value);
	}
};

template<class UserModel>
class Query{
public:
	Query& where();
	std::vector<UserModel> get(){}
	void remove();
	Query& update(std::vector<string>);
	void set(std::vector<string>);
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

	static Query<UserModel> where(std::vector<condition> conditions){
		Query<UserModel> query;
		return query;
	}

	template<typename FieldType>
	FieldType& mapToField(string colName){
		fields[colName] = new FieldType(this, colName);
		return static_cast<FieldType&>(*fields[colName]);
	}

};
}

#endif MODEL_H
