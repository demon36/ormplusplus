#ifndef INCLUDE_RESULTTABLE_H_
#define INCLUDE_RESULTTABLE_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "NullableFieldBase.h"

namespace ORMPlusPlus {

class ResultTable final {
	size_t numColumns;
	size_t numRows = 0;
	std::map<std::string, size_t> colNameToIdx;
	std::vector<const TypeInfo*> colTypes;
	std::map<size_t, NullableFieldBase> values;//map<rowIdx*numColumns+colId, value>
	static NullableFieldBase nullField;
public:
	ResultTable() = delete;

	ResultTable(const std::vector<std::string>& colNames, const std::vector<const TypeInfo*>& colTypes);
	size_t addRow();
	size_t getNumRows();
	void setFieldValue(size_t rowIdx, size_t colIdx, const char* value);
	size_t getColumnIdx(const std::string& colName);
	NullableFieldBase& getFieldValue(size_t rowIdx, size_t colIdx);
	NullableFieldBase& getFieldValue(size_t rowIdx, const std::string colName);
	template<class NullableField>
	NullableField& getFieldValue(size_t rowIdx, size_t colIdx);

	~ResultTable();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_RESULTTABLE_H_ */
