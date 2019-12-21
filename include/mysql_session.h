#ifndef INCLUDE_MYSQL_SESSION_H_
#define INCLUDE_MYSQL_SESSION_H_

#include <string>

#include "db_session_base.h"
#include "result_table.h"

#define MYSQL_DEFAULT_PORT 3306

namespace ormplusplus {

class mysql_session : public db_session_base {
private:
	void* session_ptr;
	void mysql_query(const std::string& query);
	static const type_info& get_type_info(int mysql_type_enum);
	static const type_info& get_type_info(const std::string& mysql_col_type_name);
	static const std::map<std::string, type_info> type_names_map;
public:
	mysql_session(const std::string& host, const std::string& database, const std::string& user, const std::string& password, int port = MYSQL_DEFAULT_PORT);
	bool table_exists(const std::string& name) override;
	void create_table(const std::string& name, const table_schema& schema) override;
	table_schema get_table_schema(const std::string& name) override;
	void insert(model_base& model, bool update_auto_inc_pkey = false) override;
	result_table execute_flat(const query_base& query) override;
	result_table execute_flat(const std::string& query) override;
	std::size_t execute_void(const std::string& query) override;
	virtual ~mysql_session();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_MYSQL_SESSION_H_ */
