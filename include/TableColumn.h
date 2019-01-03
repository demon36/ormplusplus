#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

#include <vector>
#include <map>
#include <typeindex>

#include "NullableField.h"

namespace ORMPlusPlus{

class TableColumn{
private:
	const std::string name;
	const std::size_t typeHash;
	int length = 0;
	int precision = 0;
	bool nullable = false;
	bool autoIncrement = false;
	std::string defaultValue;
	bool isPrimaryKey = false;
public:
	TableColumn();
	TableColumn(const std::string& name, std::size_t typeHash, int length, int precision, bool isNullable, const std::string& defaultValue, bool isPrimaryKey);
	TableColumn(const std::string& name, std::size_t typeHash);

	std::string getName();
	std::size_t getTypeHash();
	std::string getDBTypeName();
	int getLength();
	int getPrecision();
	bool isNullable();
	bool isAutoIncrement();
	bool isPrimary();
	bool isIntegral();
	bool isText();

	void setLength(int value);
	void setPrecision(int value);
	void setPrimary(bool value);
	void setNullable(bool value);
	void setAutoIncrement(bool value);

	bool operator==(const TableColumn& that);
	bool operator!=(const TableColumn& that);

};

typedef std::map<std::string, TableColumn> TableSchema;

}

#endif /* INCLUDE_TABLECOLUMN_H_ */
