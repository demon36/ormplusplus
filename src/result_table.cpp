#include <result_table.h>
#include "nullable_field.h"

namespace ormplusplus {

using namespace std;

/*
 * advantages of using nullable types here:
 * string parsing happens at the sql session layer
 * strings need not to be copied
 * move semantics can be used when forming a model from a result table
 */
result_table::result_table(const std::vector<std::string>& col_names, const std::vector<const type_info*>& _col_types)
{
	if(col_names.size() != _col_types.size()){
		throw std::runtime_error("col_names.size() != _col_type_hashes.size() in result_table ctor");
	}

	num_cols = col_names.size();
	for(size_t i = 0; i < num_cols; i++){
		col_name_to_idx[col_names[i]] = i;
	}
	col_types = _col_types;
}

size_t result_table::add_row(){
	num_rows++;
	return num_rows-1;
}

size_t result_table::get_num_rows(){
	return num_rows;
}

void result_table::set_field_value(size_t row_idx, size_t col_idx, const char* value){
	if(row_idx >= num_rows){
		throw std::out_of_range("trying to set field value for a row that does not exist");
	}
	if(col_idx >= num_cols){
		throw std::out_of_range("trying to set field value for a column that does not exist");
	}

	const type_info& column_type = *col_types[col_idx];
	size_t translated_idx = row_idx*num_cols + col_idx;

	if(value == NULL){
		values.emplace(translated_idx, nullable_field_base(column_type));//todo: fix possibly useless allocation
	}else{
		//todo: replace all of these conditions with a usage of a proper version of nullable_field(column_type)
		if(column_type == db_int::get_type_info()){
			values.emplace(translated_idx, nullable_field_base::create(column_type, stoi(value)));
		}else if(column_type == db_long::get_type_info()){
			values.emplace(translated_idx, nullable_field_base::create(column_type, stol(value)));
		}else if(column_type == db_float::get_type_info()){
			values.emplace(translated_idx, nullable_field_base::create(column_type, stof(value)));
		}else if(column_type == db_double::get_type_info()){
			values.emplace(translated_idx, nullable_field_base::create(column_type, stod(value)));
		}else if(column_type == db_string::get_type_info()){
			values.emplace(translated_idx, nullable_field_base::create(column_type, string(value)));
		}else if(column_type == db_datetime::get_type_info()){
			//TODO: query datetime with same format, ex: "1993-09-30 17:20:21"
			//TODO: use correct tz
			::tm datetime;
			strptime(value, "%Y-%m-%d %H:%M:%S", &datetime);
			values.emplace(translated_idx, nullable_field_base::create(column_type, datetime));
		}else if(column_type == db_null::get_type_info()){
			//TODO get rid of this case
		}else{
			//todo:throw exception
		}
	}

	if(row_idx > num_rows){
		num_rows = row_idx+1;
	}

}

size_t result_table::get_col_idx(const std::string& col_name){
	const auto& it = col_name_to_idx.find(col_name);
	if(it == col_name_to_idx.end()){
		throw runtime_error("column with name "+col_name+" not found");
	}
	return it->second;
}

nullable_field_base& result_table::get_field_value(size_t row_idx, size_t col_idx){
	if(row_idx >= num_rows){
		throw std::out_of_range("trying to set field value for a row that does not exist");
	}
	if(col_idx >= num_cols){
		throw std::out_of_range("trying to set field value for a column that does not exist");
	}

	auto found_value_iter = values.find(row_idx*num_cols + col_idx);
	if(found_value_iter != values.end()){
		return found_value_iter->second;
	}else{
		return null_field;
	}
}

nullable_field_base& result_table::get_field_value(size_t row_idx, const std::string& col_name){
	return get_field_value(row_idx, get_col_idx(col_name));
}

template<class primitive_type>
primitive_type& result_table::get_field_value(size_t row_idx, size_t col_idx){

}

result_table::~result_table() {
}

nullable_field_base result_table::null_field;

} /* namespace ormplusplus */
