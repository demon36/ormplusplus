#include "TableColumn.h"

#include "NullableFieldBase.h"

#define DEFAULT_STRING_LENGTH 1024
#define DEFAULT_NUM_PRECISION 10

using namespace std;

namespace ORMPlusPlus{

TableColumn::TableColumn()
: typeInfo(TypeInfo::NullType)//TODO: check the consequences
{}

TableColumn::TableColumn(const string& _name, const TypeInfo& _typeInfo, long length, long precision, bool isNullable, bool isPrimaryKey, bool autoIncrement)
: name(_name), typeInfo(_typeInfo)
{
	this->length = length;
	this->precision = precision;
	this->nullable = isNullable;
	if(isNullable){//if column is nullable then default value is NULL
		defaultValueSet = true;
	}
	this->isPrimaryKey = isPrimaryKey;
	this->autoIncrement = autoIncrement;
}

TableColumn::TableColumn(const string& _name, const TypeInfo& _typeInfo)
: name(_name), typeInfo(_typeInfo)
{
	if(isIntegral()){
		length = -1;
		precision = DEFAULT_NUM_PRECISION;
	}else if(isText()){
		length = DEFAULT_STRING_LENGTH;
		precision = -1;
	}
}

string TableColumn::getName() const { return name; }
const TypeInfo& TableColumn::getTypeInfo() const { return typeInfo; }
long TableColumn::getLength() const { return length; }
long TableColumn::getPrecision() const { return precision; }
bool TableColumn::isNullable() const { return nullable; }
bool TableColumn::hasDefaultValue() const { return defaultValueSet; }
String TableColumn::getDefaultValue() const { return defaultValue; }
bool TableColumn::isAutoIncrement() const { return autoIncrement; }
bool TableColumn::isPrimary() const { return isPrimaryKey; }

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
	if(nullable && !defaultValueSet){
		defaultValueSet = true;
	}
	//todo: this probably needs a revisit, notice this scenario:
	//use constructs TableColumn with nullable = true
	//defaultValueSet gets set to true because NULL becomes the default value
	//user calls setNullable(false)
	//now we need to clear defaultValueSet
	//but what if user already sat defaultValue to something different that NULL ?
	//todo: change defaultValue representation to be compatible with database field
	//possible values: "NULL", "34", "'a string value'", "'NULL'"
}

void TableColumn::setDefaultValue(const String& value){
	defaultValue = value;
	defaultValueSet = true;
}

void TableColumn::setAutoIncrement(bool value){
	autoIncrement = value;
}

bool TableColumn::isIntegral() const {
	return typeInfo.isIntegral;
}

bool TableColumn::isText() const {
	return typeInfo.isText;
}

bool TableColumn::operator==(const TableColumn& that){
	//TODO: update with added attributes
	if(
		this->name == that.name &&
		this->typeInfo == that.typeInfo &&
		this->nullable == that.nullable &&
		this->defaultValue == that.defaultValue &&
		this->isPrimaryKey == that.isPrimaryKey &&
		this->autoIncrement == that.autoIncrement &&
		this->length == that.length &&
		this->precision == that.precision
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
