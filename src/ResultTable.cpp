#include "ResultTable.h"

namespace ORMPlusPlus {

using namespace std;

/*
 * advantages of using nullable types here:
 * string parsing happens at the sql session layer
 * strings need not to be copied
 * move semantics can be used when forming a model from a result table
 */
ResultTable::ResultTable(const std::vector<std::string>& colNames, const std::vector<size_t>& _colTypeHashes)
{
	if(colNames.size() != _colTypeHashes.size()){
		throw std::runtime_error("colNames.size() != _colTypeHashes.size() in ResultTable ctor");
	}

	numColumns = colNames.size();
	for(size_t i = 0; i < numColumns; i++){
		colNameToIdx[colNames[i]] = i;
	}
	colTypeHashes = _colTypeHashes;
}

size_t ResultTable::addRow(){
	numRows++;
	return numRows-1;
}

size_t ResultTable::getNumRows(){
	return numRows;
}

void ResultTable::setFieldValue(size_t rowIdx, size_t colIdx, const char* value){
	if(rowIdx >= numRows){
		throw std::out_of_range("trying to set field value for a row that does not exist");
	}
	if(colIdx >= numColumns){
		throw std::out_of_range("trying to set field value for a column that does not exist");
	}

	size_t columnTypeHash = colTypeHashes[colIdx];//typeid(NullableFiled<class T>).hash_code()
	size_t translatedIdx = rowIdx*numColumns + colIdx;

	if(value == NULL){
		values.emplace(translatedIdx, NullableFieldBase(columnTypeHash));//todo: fix possibly useless allocation
	}else{
		if(columnTypeHash == typeid(int).hash_code()){
			values.emplace(translatedIdx, NullableFieldBase::create(stoi(value)));
		}else if(columnTypeHash == typeid(long).hash_code()){
			values.emplace(translatedIdx, NullableFieldBase::create(stol(value)));
		}else if(columnTypeHash == typeid(float).hash_code()){
			values.emplace(translatedIdx, NullableFieldBase::create(stof(value)));
		}else if(columnTypeHash == typeid(double).hash_code()){
			values.emplace(translatedIdx, NullableFieldBase::create(stod(value)));
		}else if(columnTypeHash == typeid(std::string).hash_code()){
			values.emplace(translatedIdx, NullableFieldBase::create(string(value)));
		}else if(columnTypeHash == typeid(::tm).hash_code()){
			//TODO: query datetime with same format, ex: "1993-09-30 17:20:21"
			//TODO: use correct tz
			::tm datetime;
			strptime(value, "%Y-%m-%d %H:%M:%S", &datetime);
			values.emplace(translatedIdx, NullableFieldBase::create(datetime));
		}else if(columnTypeHash == typeid(nullptr_t).hash_code()){
			//TODO get rid of this case
		}else{
			//todo:throw exception
		}
	}

	if(rowIdx > numRows){
		numRows = rowIdx+1;
	}

}

NullableFieldBase& ResultTable::getFieldValue(size_t rowIdx, size_t colIdx){
	if(rowIdx >= numRows){
		throw std::out_of_range("trying to set field value for a row that does not exist");
	}
	if(colIdx >= numColumns){
		throw std::out_of_range("trying to set field value for a column that does not exist");
	}

	auto foundValueIter = values.find(rowIdx*numColumns + colIdx);
	if(foundValueIter != values.end()){
		return foundValueIter->second;
	}else{
		return nullField;
	}
}

NullableFieldBase& ResultTable::getFieldValue(size_t rowIdx, const std::string colName){
	return getFieldValue(rowIdx, colNameToIdx[colName]);
}

template<class primitiveType>
primitiveType& ResultTable::getFieldValue(size_t rowIdx, size_t colIdx){

}

ResultTable::~ResultTable() {
}

NullableFieldBase ResultTable::nullField;

} /* namespace ORMPlusPlus */
