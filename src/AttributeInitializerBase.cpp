#include "AttributeInitializerBase.h"

#include "TableColumn.h"

namespace ORMPlusPlus {

AttributeInitializerBase::AttributeInitializerBase(NullableFieldBase& NFBase, TableColumn* associatedColumn)
: NFBase(NFBase), propertyColumnPtr(associatedColumn), skipColumnSetup(associatedColumn == nullptr)
{
}

NullableFieldBase& AttributeInitializerBase::getNullableFieldBaseRef() const
{
	return NFBase;
}

//TODO: when you enable using this class directly you would have to create more overloads for this
//		function and/or parse string value to int/float/etc.. and set the NFBase value as well
AttributeInitializerBase& AttributeInitializerBase::withDefault(const std::string& value){
	if(!skipColumnSetup){
		propertyColumnPtr->setDefaultValue(value);
	}
	return *this;
}

AttributeInitializerBase& AttributeInitializerBase::asPrimary(){
	if(!skipColumnSetup){
		propertyColumnPtr->setPrimary(true);
	}
	return *this;
}

//AttributeInitializerBase& asIndex();//change columnDefs, does it apply on numerical types only?

AttributeInitializerBase& AttributeInitializerBase::asNullable(bool value){
	if(!skipColumnSetup){
		propertyColumnPtr->setNullable(true);
	}
	return *this;
}

AttributeInitializerBase& AttributeInitializerBase::autoIncrement(){
	//TODO: check the type is numeric here too
	if(!skipColumnSetup){
		propertyColumnPtr->setAutoIncrement(true);
	}
	return *this;
}

AttributeInitializerBase::~AttributeInitializerBase()
{
}

} /* namespace ORMPlusPlus */
