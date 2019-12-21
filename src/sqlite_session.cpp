#include <logger.h>
#include <sqlite_session.h>
#include <typeindex>
#include <vector>
#include <sqlite3.h>

using namespace std;

namespace ormplusplus {

void* sqlite_session::sqlite_query(const std::string& query){
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2((sqlite3*)session_ptr, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
	}
	return stmt;
}

/**
 * @see https://www.sqlite.org/c3ref/c_blob.html
 */
const type_info& sqlite_session::get_type_info(int sqlite_type_id){
	//todo: revise typing setup
	//todo: use type_info here
	//todo: BROKEN LOGIC: sqlite fundamental datatypes cannot be mapped to ormplusplus data types
	switch(sqlite_type_id){
		case SQLITE_INTEGER:
			return db_int::get_type_info();
//			return typeid(long).hash_code();

		case SQLITE_FLOAT:
			return db_float::get_type_info();
//			return typeid(double).hash_code();

//		case SQLITE_TEXT:
//			return typeid(tm).hash_code();

		case SQLITE_TEXT:
			return db_string::get_type_info();

		case SQLITE_NULL:
			return db_null::get_type_info();

		default:
			throw runtime_error("unsupported type");
	}
}

const map<string, type_info> sqlite_session::type_names_map({
	{"INT", db_int::get_type_info()},
	{"INTEGER", db_long::get_type_info()},
	{"FLOAT", db_float::get_type_info()},
	{"DOUBLE", db_double::get_type_info()},
	{"VARCHAR", db_string::get_type_info()},
	{"TEXT", db_string::get_type_info()},
	{"DATETIME", db_datetime::get_type_info()},
});

const type_info& sqlite_session::get_type_info(const std::string& sqlite_col_type_name){
	string normalized_case_name = sqlite_col_type_name;
	for(char &c: normalized_case_name){
		c = (char)toupper(c);
	}

	//todo: add length to column info
	//todo: fix this bs
	if(normalized_case_name.find("VARCHAR") == 0){
		normalized_case_name = "VARCHAR";
	} else if(normalized_case_name.find("TEXT") == 0){
		normalized_case_name = "TEXT";
	}

	if(type_names_map.find(normalized_case_name) == type_names_map.end()){
		throw out_of_range("sqlite_session::get_type_info called with unsupported type name");
	}else{
		return type_names_map.find(normalized_case_name)->second;
	}
}

sqlite_session::sqlite_session(const string& db_path){
    if (sqlite3_open(db_path.c_str(), (sqlite3**)&session_ptr) != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg((sqlite3*)session_ptr));
        sqlite3_close((sqlite3*)session_ptr);
        throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
    }

	ORMLOG(logger::lvl::INFO, "connected to sqlite path " + db_path);
}

bool sqlite_session::table_exists(const string& name){
	string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='"+name+"';";
	result_table found_tables = execute_flat(query);
	return !found_tables.get_num_rows() == 0;
}

void sqlite_session::create_table(const string& name, const table_schema& schema){
	stringstream query_stream;
	query_stream << "CREATE TABLE IF NOT EXISTS `"<< name <<"`(";
	std::vector<table_column> columns_list;
	for(auto& column_entry : schema ) {
		columns_list.push_back( column_entry.second );
	}

	for(size_t i = 0; i < columns_list.size(); i++){
		string db_type_name;
		for(auto& type_name_entry : type_names_map){
			if(type_name_entry.second.wrapper_type_hash == columns_list[i].get_type_info().wrapper_type_hash){
				db_type_name = type_name_entry.first;
				break;
			}
		}

		if(db_type_name.empty()){
			throw runtime_error("trying to create table with column '"+columns_list[i].get_name()+"' having unsupported type");
		}

		query_stream << "`"<< columns_list[i].get_name() <<"` " <<  db_type_name;
		if(columns_list[i].is_text()){
			query_stream << "(" << columns_list[i].get_length() << ")";
		}

		if(columns_list[i].has_default_value()){
			if(columns_list[i].get_default_value().is_null()){
				query_stream << " DEFAULT NULL ";
			}else{
				query_stream << " DEFAULT '" << columns_list[i].get_default_value() << "' ";
			}
		}

		if(columns_list[i].is_primary_key()){
			query_stream << " PRIMARY KEY ";
		}

		if(columns_list[i].is_auto_increment()){
			query_stream << " AUTOINCREMENT ";
		}

		if(columns_list[i].is_nullable()){
			query_stream << " NULL ";
		}else{
			query_stream << " NOT NULL ";
		}

		if(i < columns_list.size()-1){
			query_stream << ", ";
		}
	}

	query_stream << ");";

	execute_void(query_stream.str());
}

table_schema sqlite_session::get_table_schema(const string& name){
	//TODO: use an ORM class for schema
	string query = "pragma table_info('"+name+"');";
	result_table result = execute_flat(query);
	table_schema schema;
	for(size_t i = 0; i < result.get_num_rows(); i++){
		string column_name = result.get_field_value(i, "name").get_value_ref<string>();
		string db_column_type = result.get_field_value(i, "type").get_value_ref<string>();
		long max_length = -1;//todo: parse number between parentheses, ex: VARCHAR(34)
		long num_precision = -1;//todo: understand how sqlite does it
		bool is_nullable = result.get_field_value(i, "notnull").get_value_ref<int>() == 0;
		bool is_pkey = result.get_field_value(i, "pk").get_value_ref<int>() == 1;
		bool is_auto_increment = true;//todo: fetch autoincrement info, will probably need to parse the table creation sql :S

		table_column temp_column(
				column_name,
				get_type_info(db_column_type),
				max_length,
				num_precision,
				is_nullable,
				is_pkey,
				is_auto_increment
		);

		if(!result.get_field_value(i, "dflt_value").is_null()){ //todo: add a test case for these conditions
			db_string default_value = result.get_field_value(i, "dflt_value").get_value_ref<string>();
			if(default_value == "NULL"){
				temp_column.set_default_value(db_string());
			}else if(temp_column.get_type_info() == db_string::get_type_info()){
				//default value is wrapped in single quotations
				temp_column.set_default_value(default_value.get_value_ref().substr(1, default_value.get_value_ref().size()-2));
			}else{
				temp_column.set_default_value(default_value);
			}
		}

		schema.emplace(column_name, temp_column);
	}
	return schema;
}

void sqlite_session::insert(model_base& model, bool update_auto_inc_pkey){
	stringstream query_stream;
	const table_schema schema = model.get_schema();
	query_stream << "INSERT INTO " << model.get_table_name();
	query_stream << " ( ";
	//will rely on map internal order, same with attribute values
	print_col_names(query_stream, model.get_schema());
	query_stream << " ) VALUES ( ";
	print_attrib_values(query_stream, model.get_schema(), model.get_attribs());
	query_stream << " );";
	if(execute_void(query_stream.str()) != 1){
		throw runtime_error("failed to insert model");
	}

	if(update_auto_inc_pkey && model.auto_inc_pkey_col_exists()){
		long last_insert_id = sqlite3_last_insert_rowid((sqlite3*)session_ptr); //  result.begin()->get(0).convert<long>();
		model.set_auto_inc_pkey(last_insert_id);
	}
	return;
}

result_table sqlite_session::execute_flat(const query_base& query){
	std::string query_string = build_query_string(query);
	return execute_flat(query_string);
}

result_table sqlite_session::execute_flat(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	sqlite3_stmt* stmt = (sqlite3_stmt*)sqlite_query(query);

	vector<string> columns;
	vector<const type_info*> column_types;

	int retval = sqlite3_step(stmt);
	if (retval != SQLITE_DONE && retval != SQLITE_ROW) {
		throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
	}

	int num_fields = sqlite3_column_count(stmt);
	for(int i = 0; i < num_fields; i++){
		columns.push_back(sqlite3_column_name(stmt, i));
		const char* sqlite_type_name = sqlite3_column_decltype(stmt, i);
		if(sqlite_type_name == NULL){
			column_types.push_back(&get_type_info(sqlite3_column_type(stmt, i)));
		} else {
			column_types.push_back(&get_type_info(sqlite_type_name));
		}
	}

	result_table flat_results(columns, column_types);
	while(retval == SQLITE_ROW){
		ORMLOG(logger::lvl::DBUG, "fetching new row");
		size_t row_idx = flat_results.add_row();
		for(int i = 0; i < num_fields; i++){
			flat_results.set_field_value(row_idx, i, (const char*)sqlite3_column_text(stmt, i));
			//todo: make use of fns like sqlite3_column_int() for better performance
		}
		retval = sqlite3_step(stmt);
	}
    sqlite3_finalize(stmt);
	return flat_results;
}

std::size_t sqlite_session::execute_void(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	sqlite3_stmt* stmt = (sqlite3_stmt*)sqlite_query(query);
	int retval = sqlite3_step(stmt);
    if (retval != SQLITE_DONE && retval != SQLITE_ROW) {
    	//todo: add app specific description to exception msgs, have a help function to wrap it
    	throw runtime_error(sqlite3_errmsg((sqlite3*)session_ptr));
    }
	return (size_t)sqlite3_changes((sqlite3*)session_ptr);
}

sqlite_session::~sqlite_session() {
	ORMLOG(logger::lvl::INFO, "disconnected from sqlite database");
	sqlite3_close((sqlite3*)session_ptr);
}

} /* namespace ormplusplus */
