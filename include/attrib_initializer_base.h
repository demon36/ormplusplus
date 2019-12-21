#ifndef INCLUDE_ATTRIB_INITIALIZER_BASE_H_
#define INCLUDE_ATTRIB_INITIALIZER_BASE_H_

#include "nullable_field_base.h"

namespace ormplusplus {

class table_column;

/**
 * Used at attrib_initializer construction
 */
class attrib_initializer_base {
private:
	nullable_field_base& nfbase;
	table_column* attrib_col_ptr;
	//denotes that the schema entry for this model property has been already added
	const bool skip_col_setup = false;
public:
	attrib_initializer_base(const attrib_initializer_base& that) = delete;
	//TODO: use table_column& + bool or table_column*
	attrib_initializer_base(nullable_field_base& nfbase, table_column* associated_col);
	nullable_field_base& get_nfbase_ref() const;
	attrib_initializer_base& with_default(const std::string& value);
	//TODO: make sure there is only one primary -maybe?-
	//TODO: make sure that primary keys are not nullable
	attrib_initializer_base& as_primary();//change column_defs
	//TODO: add support for non primary key index columns
//	attrib_initializer_base& as_index();//change column_defs, does it apply on numerical types only?
	attrib_initializer_base& as_nullable(bool value = true);
	attrib_initializer_base& auto_increment();
	virtual ~attrib_initializer_base();
};

} /* namespace ormplusplus */

#endif /* INCLUDE_ATTRIB_INITIALIZER_BASE_H_ */
