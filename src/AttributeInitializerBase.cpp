#include "AttributeInitializerBase.h"

namespace ORMPlusPlus {

AttributeInitializerBase::AttributeInitializerBase(const AttributeInitializerBase& that)
: NFBase(that.NFBase), modelInstance(that.modelInstance), columnName(that.columnName)
{
}

AttributeInitializerBase::AttributeInitializerBase(NullableFieldBase& NFBase, ModelBase& modelInstance, const std::string& attributeName)
: NFBase(NFBase), modelInstance(modelInstance), columnName(attributeName)
{
}

NullableFieldBase& AttributeInitializerBase::getNullableFieldBaseRef() const{
	return NFBase;
}

AttributeInitializerBase::~AttributeInitializerBase() {
}

} /* namespace ORMPlusPlus */
