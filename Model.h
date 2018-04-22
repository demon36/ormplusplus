#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <map>

#include "ORMPlusPlus.h"

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

#define MYSQL_DEFAULT_PORT 3306
#define DEFINE_MEMBER(DATATYPE, NAME) DATATYPE NAME = mapToField<DATATYPE>(#NAME)

namespace ORMPlusPlus{

class DB{
	static unique_ptr<Session> session;
public:
	static void initialize(string host, string database, string user, string password, int port = MYSQL_DEFAULT_PORT);
	static void executeRawQuery(string query);
};

//used for accessing renameColumn() and getColumnNames()
//without having to specify template class parameters
class ModelBase{
protected:
	map<string, void*> fields;
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
class NullableField{
	ModelBase* modelPtr = nullptr;
	string columnName = "";
	bool isNull = true;
	bool isPrimaryKey = false;
protected:
	void* valuePtr = nullptr;
	bool requireQuotes = true;
public:
	NullableField(string columnName, void* binding, ModelBase* OwnerModel){
		this->columnName = columnName;
		modelPtr = OwnerModel;
		binding = new PrimitiveType;
		valuePtr = binding;
	}

	//changes already set column name
	DerivedType& withColumnName(string newColumnName){
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
		return *static_cast<PrimitiveType*>(valuePtr);
	}

	DerivedType& withDefault(PrimitiveType value){
		*((PrimitiveType*)valuePtr) = value;
		return static_cast<DerivedType&>(*this);
	}
};

class Integer : public NullableField<Integer, int>{
	using NullableField::NullableField;
};

class String : public NullableField<String, string>{
	using NullableField::NullableField;
};

struct condition{
};

template<class UserModel>
class Query{
	Query& where();
	std::vector<UserModel> get();
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

	static std::vector<UserModel> findMany(){
		int x;
		Statement select(DB);
		select << "SELECT Name, Address, Age FROM Person",
			into(x),
			range(0, 1);
//			vector<Derived> list;
//			Derived d;
//			list.push_back(d);
//			return list;
	}

	template<typename T>
	T mapToField(string colName){
		fields[colName] = nullptr;
		T member(colName, fields[colName], this);
		return member;
	}

};
}

#endif MODEL_H
