#include "TableColumn.h"

#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 10

namespace ORMPlusPlus{

std::vector<std::string> TableColumn::DataTypeNames = {"INT", "VARCHAR"};

TableColumn::TableColumn(){}

TableColumn::TableColumn(std::string name, DataType type, int length, int precision, bool isNullable, std::string defaultValue, bool isPrimaryKey){
	this->name = name;
	this->type = type;
	this->length = length;
	this->precision = precision;
	this->isNullable = isNullable;
	this->defaultValue = defaultValue;
	this->isPrimaryKey = isPrimaryKey;
}

TableColumn::TableColumn(std::string name, DataType type){
	this->name = name;
	this->type = type;
	if(type == DataType::_Integer){
		length = 0;
		precision = DEFAULT_NUM_PRECISION;
	}else if(type == DataType::_String){
		length = DEFAULT_STRING_LENGTH;
		precision = 0;
	}
}

std::string TableColumn::getName(){ return name; }
DataType TableColumn::getType(){ return type; }
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
