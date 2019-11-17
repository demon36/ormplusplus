#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

#include <map>

#include "NullableField.h"

namespace ORMPlusPlus{

/**
 * 	responsible for storing column data that is used to create a table
	such as column name, data type, nullable, primary, autoincrement, etc..
 */
class TableColumn{
private:
	const std::string name;
	const TypeInfo& typeInfo;
	long length = -1;//todo: use nullable type here too ?
	long precision = -1;
	bool nullable = false;
	bool defaultValueSet = false;
	String defaultValue;//default value can be non-existent, NULL or "NULL" (or any other string)
	bool isPrimaryKey = false;
	bool autoIncrement = false;
public:
	TableColumn();
	TableColumn(const std::string& _name, const TypeInfo& _typeInfo, long length, long precision, bool isNullable, bool isPrimaryKey, bool autoIncrement);
	TableColumn(const std::string& _name, const TypeInfo& _typeInfo);

	std::string getName() const;
	const TypeInfo& getTypeInfo() const;
	long getLength() const;
	long getPrecision() const;
	bool isNullable() const;
	bool hasDefaultValue() const;
	String getDefaultValue() const;
	bool isPrimary() const;
	bool isAutoIncrement() const;
	bool isIntegral() const;
	bool isText() const;

	void setLength(int value);
	void setPrecision(int value);
	void setPrimary(bool value);
	void setNullable(bool value);
	void setDefaultValue(const String& value);
	void setAutoIncrement(bool value);

	bool operator==(const TableColumn& that);
	bool operator!=(const TableColumn& that);

};

typedef std::map<std::string, TableColumn> TableSchema;

}

#endif /* INCLUDE_TABLECOLUMN_H_ */
