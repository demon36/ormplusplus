#include "TableColumn.h"

#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 10

namespace ORMPlusPlus{

TableColumn::TableColumn()
: typeHash(typeid(nullptr).hash_code())//TODO: check the consequences
{}

TableColumn::TableColumn(const std::string& name_, const std::size_t typeHash_, int length, int precision, bool isNullable, const std::string& defaultValue, bool isPrimaryKey)
: name(name_), typeHash(typeHash_)//TODO: add more
{
	this->length = length;
	this->precision = precision;
	this->isNullable = isNullable;
	this->defaultValue = defaultValue;
	this->isPrimaryKey = isPrimaryKey;
}

TableColumn::TableColumn(const std::string& name_, const std::size_t typeHash_)
: name(name_), typeHash(typeHash_)
{
	if(isIntegral()){
		length = 0;
		precision = DEFAULT_NUM_PRECISION;
	}else if(isText()){
		length = DEFAULT_STRING_LENGTH;
		precision = 0;
	}
}

std::string TableColumn::getName(){ return name; }
const std::size_t& TableColumn::getTypeHash(){ return typeHash; }
std::string TableColumn::getDBTypeName(){ return NullableFieldBase::typeInfoMap.at(typeHash).DBName; }
int TableColumn::getLength(){ return length; }
int TableColumn::getPrecision(){ return precision; }
bool TableColumn::isPrimary(){ return isPrimaryKey; }

bool TableColumn::isIntegral(){
	//TODO: check key exists
	return NullableFieldBase::typeInfoMap.at(typeHash).isIntegral;
}

bool TableColumn::isText(){
	return NullableFieldBase::typeInfoMap.at(typeHash).isText;
}

bool TableColumn::operator==(const TableColumn& that){
	if(
		this->name == that.name &&
		this->typeHash == that.typeHash &&
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
