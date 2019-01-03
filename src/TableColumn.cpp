#include "TableColumn.h"

#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 10

using namespace std;

namespace ORMPlusPlus{

TableColumn::TableColumn()
: typeHash(typeid(nullptr).hash_code())//TODO: check the consequences
{}

TableColumn::TableColumn(const string& name_, size_t typeHash_, int length, int precision, bool isNullable, const string& defaultValue, bool isPrimaryKey)
: name(name_), typeHash(typeHash_)
{
	this->length = length;
	this->precision = precision;
	this->nullable = isNullable;
	this->defaultValue = defaultValue;
	this->isPrimaryKey = isPrimaryKey;
}

TableColumn::TableColumn(const string& name_, size_t typeHash_)
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

string TableColumn::getName(){ return name; }
size_t TableColumn::getTypeHash(){ return typeHash; }
string TableColumn::getDBTypeName(){ return NullableFieldBase::typeInfoMap.at(typeHash).DBName; }
int TableColumn::getLength(){ return length; }
int TableColumn::getPrecision(){ return precision; }
bool TableColumn::isNullable(){ return nullable; }
bool TableColumn::isAutoIncrement(){ return autoIncrement; }
bool TableColumn::isPrimary(){ return isPrimaryKey; }

void TableColumn::setLength(int value){
	if(!isText()){
		throw runtime_error("trying to set length on non-text type");
	}
	length = value;
}

void TableColumn::setPrecision(int value){
	if(!isText()){
		throw runtime_error("trying to set precision on non-numeric type");
	}
	length = value;
}

//TODO: should this be setAsIndex ??
void TableColumn::setPrimary(bool value){
	isPrimaryKey = value;
}

void TableColumn::setNullable(bool value){
	nullable = value;
}

void TableColumn::setAutoIncrement(bool value){
	autoIncrement = value;
}

bool TableColumn::isIntegral(){
	//TODO: check key exists
	//TODO: maybe instead of using TypeInfo struct we need to some sets
	//		each set contains ids for types holding some property
	return NullableFieldBase::typeInfoMap.at(typeHash).isIntegral;
}

bool TableColumn::isText(){
	return NullableFieldBase::typeInfoMap.at(typeHash).isText;
}

bool TableColumn::operator==(const TableColumn& that){
	//TODO: update with added attributes
	if(
		this->name == that.name &&
		this->typeHash == that.typeHash &&
		this->nullable == that.nullable &&
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
