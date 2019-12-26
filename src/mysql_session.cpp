#include <logger.h>
#include <typeindex>
#include <vector>

#include <mysql/mysql.h>
#include <mysql_session.h>


using namespace std;

namespace ormplusplus {

void mysql_session::mysql_query_(const std::string& query){
	if (mysql_query((st_mysql*)session_ptr, query.c_str())) {
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}
}

const type_info& mysql_session::get_type_info(int mysql_type_enum){
	//todo: revise typing setup
	enum_field_types mysql_type = (enum_field_types)mysql_type_enum;
	switch(mysql_type){
//		case MYSQL_TYPE_BIT://todo: support bool
		case MYSQL_TYPE_TINY:
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			return db_int::get_type_info();

		case MYSQL_TYPE_LONGLONG:
			return db_long::get_type_info();


		case MYSQL_TYPE_FLOAT:
			return db_float::get_type_info();
		case MYSQL_TYPE_DOUBLE:
		case MYSQL_TYPE_DECIMAL:
			return db_double::get_type_info();

		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_NEWDATE:
			return db_datetime::get_type_info();

		case MYSQL_TYPE_VARCHAR:
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_BLOB://todo: support blob
			return db_string::get_type_info();

		case MYSQL_TYPE_NULL:
			return db_null::get_type_info();

		default:
			throw runtime_error("unsupported type");
	}
}

const map<string, type_info> mysql_session::type_names_map({
	{"INT", db_int::get_type_info()},
	{"BIGINT", db_long::get_type_info()},
	{"FLOAT", db_float::get_type_info()},
	{"DOUBLE", db_double::get_type_info()},
	{"VARCHAR", db_string::get_type_info()},
	{"DATETIME", db_datetime::get_type_info()},
});

const type_info& mysql_session::get_type_info(const std::string& mysql_type_name){
	string normalized_case_name = mysql_type_name;
	for(char &c: normalized_case_name){
		c = (char)toupper(c);
	}

	if(type_names_map.find(normalized_case_name) == type_names_map.end()){
		//todo: add exception macro with context details
		throw out_of_range("mysql_session::get_type_info called with unsupported type name");
	}else{
		return type_names_map.find(normalized_case_name)->second;
	}
}

mysql_session::mysql_session(const string& host, const string& database, const string& user, const string& password, int port){
	session_ptr = mysql_init(NULL);

	if(session_ptr == NULL){
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}

	//todo: use compression
	if (mysql_real_connect((st_mysql*)session_ptr, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0) == NULL){
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}

	ORMLOG(logger::lvl::INFO, "connected to mysql server " + host + ":" + to_string(port));
}

bool mysql_session::table_exists(const string& name){
	string query = "SHOW TABLES LIKE '"+name+"';";
	result_table found_tables = execute_flat(query);
	return !found_tables.get_num_rows() == 0;
}

void mysql_session::create_table(const string& name, const table_schema& schema){
	stringstream query_stream;
	query_stream << "CREATE TABLE IF NOT EXISTS `"<< name <<"`(";
	std::vector<table_column> cols_list;
	for(auto& column_entry : schema ) {
		cols_list.push_back( column_entry.second );
	}


	for(size_t i = 0; i < cols_list.size(); i++){
		string db_type_name;
		for(auto& type_name_entry : type_names_map){
			if(type_name_entry.second.wrapper_type_hash == cols_list[i].get_type_info().wrapper_type_hash){
				db_type_name = type_name_entry.first;
				break;
			}
		}

		if(db_type_name.empty()){
			throw runtime_error("trying to create table with column '"+cols_list[i].get_name()+"' having unsupported type");
		}

		query_stream << "`"<< cols_list[i].get_name() <<"` " <<  db_type_name;
		if(cols_list[i].is_text()){
			query_stream << "(" << cols_list[i].get_length() << ")";
		}
		if(cols_list[i].is_auto_increment()){
			query_stream << " AUTO_INCREMENT ";
		}

		if(cols_list[i].has_default_value()){
			if(cols_list[i].get_default_value().is_null){
				query_stream << " DEFAULT NULL ";
			}else{
				query_stream << " DEFAULT '" << cols_list[i].get_default_value().val << "' ";
			}
		}

		if(cols_list[i].is_primary_key()){
			query_stream << " PRIMARY KEY ";
		}
		if(cols_list[i].is_nullable()){
			query_stream << " NULL ";
		}else{
			query_stream << " NOT NULL ";
		}
		if(i < cols_list.size()-1){
			query_stream << ", ";
		}
	}

	query_stream << ");";

	execute_void(query_stream.str());
}

table_schema mysql_session::get_table_schema(const string& name){
	//TODO: use an ORM class for schema
	string query = "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, NUMERIC_PRECISION, IS_NULLABLE, COLUMN_DEFAULT, COLUMN_KEY, EXTRA "
			"FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+name+"';";
	result_table result = execute_flat(query);
	table_schema schema;
	for(size_t i = 0; i < result.get_num_rows(); i++){
		string col_name = result.get_raw_field_value(i, "COLUMN_NAME").val;
		string db_col_type = result.get_raw_field_value(i, "DATA_TYPE").val;
		string extra = result.get_raw_field_value(i, "EXTRA").val;
		//todo: clean this mess
		long max_len = result.get_raw_field_value(i, "CHARACTER_MAXIMUM_LENGTH").is_null ? -1 : stol(result.get_raw_field_value(i, "CHARACTER_MAXIMUM_LENGTH").val.c_str());
		long num_precision = result.get_raw_field_value(i, "NUMERIC_PRECISION").is_null ? -1 : stol(result.get_raw_field_value(i, "NUMERIC_PRECISION").val.c_str());
		bool is_nullable = result.get_raw_field_value(i, "IS_NULLABLE").val == "YES";
		bool is_pkey = result.get_raw_field_value(i, "COLUMN_KEY").val == "PRI";
		bool is_auto_inc = extra.find("auto_increment") != extra.npos;

		table_column tmp_col(
				col_name,
				get_type_info(db_col_type),
				max_len,
				num_precision,
				is_nullable,
				is_pkey,
				is_auto_inc
		);

		if(!result.get_raw_field_value(i, "COLUMN_DEFAULT").is_null){
			db_string default_value = result.get_raw_field_value(i, "COLUMN_DEFAULT").val;
			if(default_value == "NULL"){
				tmp_col.set_default_value("", true);//todo: enhance this
			}else if(tmp_col.get_type_info() == db_string::get_type_info()){
				//default value is wrapped in single quotations
				tmp_col.set_default_value(default_value.get_value_ref().substr(1, default_value.get_value_ref().size()-2), false);
			}else{
				tmp_col.set_default_value(default_value, false);
			}
		}

		schema.emplace(col_name, tmp_col);
	}
	return schema;
}

void mysql_session::insert(model_base& model, bool update_auto_inc_pkey){
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
		long last_insert_id = mysql_insert_id((st_mysql*)session_ptr); //  result.begin()->get(0).convert<long>();
		model.set_auto_inc_pkey(last_insert_id);
	}
	return;
}

result_table mysql_session::execute_flat(const query_base& query){
	std::string query_string = build_query_string(query);
	return execute_flat(query_string);
}

result_table mysql_session::execute_flat(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	mysql_query_(query);

	MYSQL_RES* result = mysql_store_result((st_mysql*)session_ptr);

	if(result == NULL){
		throw runtime_error(mysql_error((st_mysql*)session_ptr));
	}

	MYSQL_ROW row;
	MYSQL_FIELD *field;
	vector<string> columns;
	vector<const type_info*> col_types;

	int num_fields = mysql_num_fields(result);

	while((field = mysql_fetch_field(result)) != NULL){
		columns.push_back(field->name);
		col_types.push_back(&get_type_info(field->type));
	}

	result_table flat_results(columns, col_types);

	while((row = mysql_fetch_row(result)) != NULL){
		ORMLOG(logger::lvl::DBUG, "fetching new row");
		size_t row_idx = flat_results.add_row();
		for(int i = 0; i < num_fields; i++){
			flat_results.set_field_value(row_idx, i, row[i]);
		}
	}

	mysql_free_result(result);
	return flat_results;
}

std::size_t mysql_session::execute_void(const std::string& query){
	ORMLOG(logger::lvl::DBUG, "executing query : " + query);
	mysql_query_(query);
	return (size_t)mysql_affected_rows((st_mysql*)session_ptr);
}

mysql_session::~mysql_session() {
	ORMLOG(logger::lvl::INFO, "disconnected from mysql server ");
	mysql_close((st_mysql*)session_ptr);
}

} /* namespace ormplusplus */
