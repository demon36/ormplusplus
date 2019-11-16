#ifndef INCLUDE_ATTRIBUTEINITIALIZERBASE_H_
#define INCLUDE_ATTRIBUTEINITIALIZERBASE_H_

#include "NullableFieldBase.h"

namespace ORMPlusPlus {

class TableColumn;

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
	AttributeInitializerBase& withDefault(const std::string& value);
	//TODO: make sure there is only one primary -maybe?-
	//TODO: make sure that primary keys are not nullable
	AttributeInitializerBase& asPrimary();//change columnDefs
	//TODO: add support for non primary key index columns
//	AttributeInitializerBase& asIndex();//change columnDefs, does it apply on numerical types only?
	AttributeInitializerBase& asNullable(bool value = true);
	AttributeInitializerBase& autoIncrement();
	virtual ~AttributeInitializerBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_ATTRIBUTEINITIALIZERBASE_H_ */
