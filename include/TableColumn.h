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
	const std::size_t typeHash;
	long length = -1;//todo: use nullable type here too ?
	long precision = -1;
	bool nullable = false;
	String defaultValue;
	bool isPrimaryKey = false;
	bool autoIncrement = false;
public:
	TableColumn();
	TableColumn(const std::string& name, std::size_t typeHash, long length, long precision, bool isNullable, const String& defaultValue, bool isPrimaryKey, bool autoIncrement);
	TableColumn(const std::string& name, std::size_t typeHash);

	std::string getName() const;
	std::size_t getTypeHash() const;
	std::string getDBTypeName() const;
	long getLength() const;
	long getPrecision() const;
	bool isNullable() const;
	String getDefaultValue() const;
	bool isPrimary() const;
	bool isAutoIncrement() const;
	bool isIntegral() const;
	bool isText() const;

	void setLength(int value);
	void setPrecision(int value);
	void setPrimary(bool value);
	void setNullable(bool value);
	void setDefaultValue(const std::string& value);
	void setAutoIncrement(bool value);

	bool operator==(const TableColumn& that);
	bool operator!=(const TableColumn& that);

};

typedef std::map<std::string, TableColumn> TableSchema;

}

#endif /* INCLUDE_TABLECOLUMN_H_ */
