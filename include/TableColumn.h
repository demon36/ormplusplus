#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

#include <vector>
#include <map>

#include "NullableField.h"

namespace ORMPlusPlus{

class TableColumn{
private:
	static std::vector<std::string> DataTypeNames;
	std::string name;
	DataType type;
	int length = 0;
	int precision = 0;
	bool isNullable = false;
	std::string defaultValue;
	bool isPrimaryKey = false;
public:
	TableColumn();
	TableColumn(std::string name, DataType type, int length, int precision, bool isNullable, std::string defaultValue, bool isPrimaryKey);
	TableColumn(std::string name, DataType type);

	std::string getName();
	DataType getType();
	std::string getTypeName();
	int getLength();
	int getPrecision();
	bool isPrimary();

	bool operator==(const TableColumn& that);
	bool operator!=(const TableColumn& that);

};

typedef std::map<std::string, TableColumn> TableSchema;

}

#endif /* INCLUDE_TABLECOLUMN_H_ */
