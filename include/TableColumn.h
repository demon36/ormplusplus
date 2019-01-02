#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

#include <vector>
#include <map>
#include <typeindex>

#include "NullableField.h"

namespace ORMPlusPlus{

class TableColumn{
private:
	//TODO: should use type_info hash instead ?
	static std::map<std::type_index, std::string> DataTypeNames;
	std::string name;
	const std::type_info& type;
	int length = 0;
	int precision = 0;
	bool isNullable = false;
	std::string defaultValue;
	bool isPrimaryKey = false;
public:
	TableColumn();
	TableColumn(const std::string& name, const std::type_info& type, int length, int precision, bool isNullable, const std::string& defaultValue, bool isPrimaryKey);
	TableColumn(const std::string& name, const std::type_info& type);

	std::string getName();
	const std::type_info& getType();
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
