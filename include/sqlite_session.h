#ifndef INCLUDE_SQLITE_SESSION_H_
#define INCLUDE_SQLITE_SESSION_H_

#include <string>

#include "db_session_base.h"
#include "result_table.h"

namespace ormplusplus {

class sqlite_session : public db_session_base {
private:
	void* session_ptr;
	void* sqlite_query(const std::string& query);
	bool supports_autoincrement_check();
	//todo: add these functions to db_session_base ?
	static const type_info& get_type_info(int sqlite_type_enum);
	static const type_info& get_type_info(const std::string& sqlite_col_type_name);
	static const std::map<std::string, type_info> type_names_map;
public:
	sqlite_session(const std::string& db_path);
	bool table_exists(const std::string& name) override;
	void create_table(const std::string& name, const table_schema& schema) override;
	table_schema get_table_schema(const std::string& name) override;
	void insert(model_base& model, bool update_auto_inc_pkey = false) override;
	result_table execute_flat(const query_base& query) override;
	result_table execute_flat(const std::string& query) override;
	std::size_t execute_void(const std::string& query) override;
	virtual ~sqlite_session();
};

} /* namespace ormplusplus */

#endif /* INCLUDE_SQLITE_SESSION_H_ */
