#ifndef INCLUDE_TABLE_COLUMN_H_
#define INCLUDE_TABLE_COLUMN_H_

#include <map>

#include "nullable_field.h"

namespace ormplusplus{

/**
 * 	responsible for storing column data that is used to create a table
	such as column name, data type, nullable, primary, autoincrement, etc..
 */
class table_column{
private:
	const std::string name;
	const type_info& type;
	long length = -1;//todo: use nullable type for length and precision to avoid comparing them when checking table schema exists
	long precision = -1;
	bool nullable = false;
	bool default_value_set = false;
	db_string default_value;//default value can be non-existent, NULL or "NULL" (or any other string)
	bool is_pkey = false;
	bool is_auto_inc = false;
public:
	table_column();
	table_column(const std::string& _name, const type_info& _type_info, long _length, long _precision, bool _is_nullable, bool _is_pkey, bool _is_auto_inc);
	table_column(const std::string& _name, const type_info& _type_info);

	std::string get_name() const;
	const type_info& get_type_info() const;
	long get_length() const;
	long get_precision() const;
	bool is_nullable() const;
	bool has_default_value() const;
	db_string get_default_value() const;
	bool is_primary_key() const;//todo: possible to make these attribs public ?
	bool is_auto_increment() const;
	bool is_integral() const;
	bool is_text() const;

	void set_length(int value);
	void set_precision(int value);
	void set_primary(bool value);
	void set_nullable(bool value);
	void set_default_value(const db_string& value);
	void set_auto_increment(bool value);

	bool operator==(const table_column& that);
	bool operator!=(const table_column& that);

};

typedef std::map<std::string, table_column> table_schema;

}

#endif /* INCLUDE_TABLE_COLUMN_H_ */
