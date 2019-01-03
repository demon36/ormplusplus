#ifndef INCLUDE_ATTRIBUTEINITIALIZERBASE_H_
#define INCLUDE_ATTRIBUTEINITIALIZERBASE_H_

#include "TableColumn.h"
#include "NullableField.h"

namespace ORMPlusPlus {

/**
 * Used at NullableField construction
 */
class AttributeInitializerBase {
private:
	NullableFieldBase& NFBase;
	TableColumn* propertyColumnPtr;
	//denotes that the schema entry for this model property has been already added
	const bool skipColumnSetup = false;
public:
	AttributeInitializerBase(const AttributeInitializerBase& that) = delete;
	//TODO: use TableColumn& + bool or TableColumn*
	AttributeInitializerBase(NullableFieldBase& NFBase, TableColumn* associatedColumn);
	NullableFieldBase& getNullableFieldBaseRef() const;
	//TODO: make sure there is only one primary -maybe?-
	AttributeInitializerBase& asPrimary();//change columnDefs
	AttributeInitializerBase& asIndex();//change columnDefs, does it apply on numerical types only?
	AttributeInitializerBase& nullable(bool value = true);
	AttributeInitializerBase& autoIncrement();
	virtual ~AttributeInitializerBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_ATTRIBUTEINITIALIZERBASE_H_ */
