#ifndef INCLUDE_ATTRIBUTEINITIALIZERBASE_H_
#define INCLUDE_ATTRIBUTEINITIALIZERBASE_H_

#include "ModelBase.h"
#include "TableColumn.h"
#include "NullableField.h"

namespace ORMPlusPlus {

class AttributeInitializerBase {
private:
	NullableFieldBase& NFBase;
	std::string columnName;
	ModelBase& modelInstance;
public:
	AttributeInitializerBase(const AttributeInitializerBase& that);
	AttributeInitializerBase(NullableFieldBase& NFBase, ModelBase& modelInstance, const std::string& attributeName);
	NullableFieldBase& getNullableFieldBaseRef() const;
	virtual ~AttributeInitializerBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_ATTRIBUTEINITIALIZERBASE_H_ */
