#include "TableColumn.h"

#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 10

namespace ORMPlusPlus{

//TODO: this info should be moved to type itself
std::map<std::type_index, std::string> TableColumn::DataTypeNames = {
		{typeid(Integer), "INT"},
		{typeid(String), "VARCHAR"},
};

TableColumn::TableColumn()
: type(typeid(nullptr))
{}

TableColumn::TableColumn(const std::string& name_, const std::type_info& type_, int length, int precision, bool isNullable, const std::string& defaultValue, bool isPrimaryKey)
: type(type_), name(name_)//TODO: add more
{
	this->length = length;
	this->precision = precision;
	this->isNullable = isNullable;
	this->defaultValue = defaultValue;
	this->isPrimaryKey = isPrimaryKey;
}

TableColumn::TableColumn(const std::string& name_, const std::type_info& type_)
: type(type_), name(name_)
{
	if(type == typeid(Integer)){
		length = 0;
		precision = DEFAULT_NUM_PRECISION;
	}else if(type == typeid(String)){
		length = DEFAULT_STRING_LENGTH;
		precision = 0;
	}
}

std::string TableColumn::getName(){ return name; }
const std::type_info& TableColumn::getType(){ return type; }
std::string TableColumn::getTypeName(){ return DataTypeNames[type]; }
int TableColumn::getLength(){ return length; }
int TableColumn::getPrecision(){ return precision; }
bool TableColumn::isPrimary(){ return isPrimaryKey; }

bool TableColumn::operator==(const TableColumn& that){
	if(
			this->name == that.name &&
			this->type == that.type &&
			this->isNullable == that.isNullable &&
			this->defaultValue == that.defaultValue &&
			this->isPrimaryKey == that.isPrimaryKey
	){
		return true;
	}else{
		return false;
	}
}

bool TableColumn::operator!=(const TableColumn& that){
	return !operator==(that);
}

}
