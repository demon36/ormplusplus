#include "AttributeInitializerBase.h"

namespace ORMPlusPlus {

AttributeInitializerBase::AttributeInitializerBase(NullableFieldBase& NFBase, TableColumn* associatedColumn)
: NFBase(NFBase), propertyColumnPtr(associatedColumn), skipColumnSetup(associatedColumn == nullptr)
{
}

NullableFieldBase& AttributeInitializerBase::getNullableFieldBaseRef() const
{
	return NFBase;
}

AttributeInitializerBase& AttributeInitializerBase::asPrimary(){
	if(!skipColumnSetup){
		propertyColumnPtr->setPrimary(true);
	}
	return *this;
}

//AttributeInitializerBase& asIndex();//change columnDefs, does it apply on numerical types only?

AttributeInitializerBase& AttributeInitializerBase::nullable(bool value){
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
