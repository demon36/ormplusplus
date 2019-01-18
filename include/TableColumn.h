#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

#include <vector>
#include <map>
#include <typeindex>

namespace ORMPlusPlus{

class TableColumn{
private:
	const std::string name;
	const std::size_t typeHash;
	int length = 0;
	int precision = 0;
	bool nullable = false;
	std::string defaultValue;
	bool isPrimaryKey = false;
	bool autoIncrement = false;
public:
	TableColumn();
	TableColumn(const std::string& name, std::size_t typeHash, int length, int precision, bool isNullable, const std::string& defaultValue, bool isPrimaryKey, bool autoIncrement);
	TableColumn(const std::string& name, std::size_t typeHash);

	std::string getName() const;
	std::size_t getTypeHash() const;
	std::string getDBTypeName() const;
	int getLength() const;
	int getPrecision() const;
	bool isNullable() const;
	std::string getDefaultValue() const;
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
