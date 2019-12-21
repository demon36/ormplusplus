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
result_table::result_table(const std::vector<std::string>& colNames, const std::vector<const type_info*>& _colTypes)
{
	if(colNames.size() != _colTypes.size()){
		throw std::runtime_error("colNames.size() != _colTypeHashes.size() in ResultTable ctor");
	}

	num_cols = colNames.size();
	for(size_t i = 0; i < num_cols; i++){
		col_name_to_idx[colNames[i]] = i;
	}
	col_types = _colTypes;
}

size_t result_table::add_row(){
	num_rows++;
	return num_rows-1;
}

size_t result_table::get_num_rows(){
	return num_rows;
}

void result_table::set_field_value(size_t rowIdx, size_t colIdx, const char* value){
	if(rowIdx >= num_rows){
		throw std::out_of_range("trying to set field value for a row that does not exist");
	}
	if(colIdx >= num_cols){
		throw std::out_of_range("trying to set field value for a column that does not exist");
	}

	const type_info& columnType = *col_types[colIdx];
	size_t translatedIdx = rowIdx*num_cols + colIdx;

	if(value == NULL){
		values.emplace(translatedIdx, nullable_field_base(columnType));//todo: fix possibly useless allocation
	}else{
		//todo: replace all of these conditions with a usage of a proper version of NullableField(columnType)
		if(columnType == Integer::getTypeInfo()){
			values.emplace(translatedIdx, nullable_field_base::create(columnType, stoi(value)));
		}else if(columnType == Long::getTypeInfo()){
			values.emplace(translatedIdx, nullable_field_base::create(columnType, stol(value)));
		}else if(columnType == Float::getTypeInfo()){
			values.emplace(translatedIdx, nullable_field_base::create(columnType, stof(value)));
		}else if(columnType == Double::getTypeInfo()){
			values.emplace(translatedIdx, nullable_field_base::create(columnType, stod(value)));
		}else if(columnType == String::getTypeInfo()){
			values.emplace(translatedIdx, nullable_field_base::create(columnType, string(value)));
		}else if(columnType == DateTime::getTypeInfo()){
			//TODO: query datetime with same format, ex: "1993-09-30 17:20:21"
			//TODO: use correct tz
			::tm datetime;
			strptime(value, "%Y-%m-%d %H:%M:%S", &datetime);
			values.emplace(translatedIdx, nullable_field_base::create(columnType, datetime));
		}else if(columnType == Null::getTypeInfo()){
			//TODO get rid of this case
		}else{
			//todo:throw exception
		}
	}

	if(rowIdx > num_rows){
		num_rows = rowIdx+1;
	}

}

size_t result_table::get_col_idx(const std::string& colName){
	const auto& it = col_name_to_idx.find(colName);
	if(it == col_name_to_idx.end()){
		throw runtime_error("column with name "+colName+" not found");
	}
	return it->second;
}

nullable_field_base& result_table::get_field_value(size_t rowIdx, size_t colIdx){
	if(rowIdx >= num_rows){
		throw std::out_of_range("trying to set field value for a row that does not exist");
	}
	if(colIdx >= num_cols){
		throw std::out_of_range("trying to set field value for a column that does not exist");
	}

	auto foundValueIter = values.find(rowIdx*num_cols + colIdx);
	if(foundValueIter != values.end()){
		return foundValueIter->second;
	}else{
		return null_field;
	}
}

nullable_field_base& result_table::get_field_value(size_t rowIdx, const std::string colName){
	return get_field_value(rowIdx, get_col_idx(colName));
}

template<class primitiveType>
primitiveType& result_table::get_field_value(size_t rowIdx, size_t colIdx){

}

result_table::~result_table() {
}

nullable_field_base result_table::null_field;

} /* namespace ORMPlusPlus */
