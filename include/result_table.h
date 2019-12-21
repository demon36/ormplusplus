#ifndef INCLUDE_RESULT_TABLE_H_
#define INCLUDE_RESULT_TABLE_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "nullable_field_base.h"

namespace ormplusplus {

class result_table final {
	size_t num_cols;
	size_t num_rows = 0;
	std::map<std::string, size_t> col_name_to_idx;
	std::vector<const type_info*> col_types;
	std::map<size_t, nullable_field_base> values;//map<row_idx*num_cols+col_id, value>
	static nullable_field_base null_field;
public:
	result_table() = delete;

	result_table(const std::vector<std::string>& col_names, const std::vector<const type_info*>& _col_types);
	size_t add_row();
	size_t get_num_rows();
	void set_field_value(size_t row_idx, size_t col_idx, const char* value);
	size_t get_col_idx(const std::string& col_name);
	nullable_field_base& get_field_value(size_t row_idx, size_t col_idx);
	nullable_field_base& get_field_value(size_t row_idx, const std::string& col_name);
	template<class nullable_field_t>
	nullable_field_t& get_field_value(size_t row_idx, size_t col_idx);

	~result_table();
};

} /* namespace ormplusplus */

#endif /* INCLUDE_RESULT_TABLE_H_ */
