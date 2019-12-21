#include <logger.h>
#include "ormplusplus.h"
#include <iomanip>

using namespace std;
using namespace ormplusplus;

#define ASSERT(expression) print_test_result(#expression, expression)

void print_test_result(const string& expression, bool result){
	ostream& target_stream = result ? cout : cerr ;
	target_stream << "assertion `" << left << setw(60) << expression + "` ";
	target_stream << " >>>>>>> " << right << setw(20) << (result ? "succeeded" : "failed") << endl;
}

class client : public model<client>
{
public:
	DEFINE_ATTRIB(db_long, id).auto_increment().as_primary();
	DEFINE_ATTRIB(db_string, name).with_default("nameless");
	DEFINE_ATTRIB(db_int, age).with_default(5).as_nullable();
	DEFINE_ATTRIB(db_int, height);
	DEFINE_ATTRIB(db_datetime, dob).as_nullable(false);

	constexpr static const char* _id = "asdf"; //use instead of 2nd template parameter ??
};

class mini_bus : public model<mini_bus, _("minibuses")>
{
public:
	DEFINE_ATTRIB(db_int, id).auto_increment().as_primary();
	DEFINE_ATTRIB(db_string, model).with_default("my_van");
};


void test_model_definition(){
	ASSERT(mini_bus::get_table_name() == "minibuses");
	ASSERT(client::get_table_name() == "client");
	ASSERT(db::is_user_model_class<client>());
	ASSERT(!db::is_user_model_class<db_int>());

	client c;
	ASSERT(c.id.is_null());
	ASSERT(c.name == "nameless");
	ASSERT(c.age == 5);
	ASSERT(c.age < 6);
	ASSERT(c.height.is_null());

	const table_schema& schema = client::get_schema();
	ASSERT(schema.at("id").is_auto_increment());
	ASSERT(schema.at("id").is_primary_key());
	ASSERT(schema.at("name").get_default_value() == "nameless");
	ASSERT(schema.at("name").is_text());
	ASSERT(!schema.at("name").is_integral());
	ASSERT(schema.at("age").get_default_value() == "5");
	ASSERT(schema.at("age").is_nullable());
	ASSERT(!schema.at("height").is_auto_increment());
	ASSERT(!schema.at("height").is_nullable());
	ASSERT(schema.at("height").is_integral());
	ASSERT(schema.at("height").has_default_value() == false);
	ASSERT(!schema.at("height").is_primary_key());
	ASSERT(!schema.at("height").is_text());
}

void assert_table_creation(){
	db::drop_table<client>();
	ASSERT(!db::table_exists<client>());
	db::create_table<client>();
	ASSERT(db::table_exists<client>());
	ASSERT(db::table_exists<client>(true));
}

void test_single_insert_and_select(){
	client c0;
	c0.age = 15;
	c0.name = "robert";
	c0.height = 167;
	::tm dobtm;
	strptime("1993-09-30 17:20:21", "%Y-%m-%d %H:%M:%S", &dobtm);
	c0.dob = dobtm;
	c0.insert(true);

	client c1 = client::where({
		{"id", "=", c0.id}
	}).select_one();

	ASSERT(c0.equals(c1));//todo: fix this
}

void test_multi_insert_and_select(){
	std::vector<client> clients(10);
}

int main(int argc, char** argv)
{
	logger::set_level(logger::lvl::DBUG);
	test_model_definition();
	map<string, shared_ptr<db_session_base>> db_sessions = {
		{"mysql", make_shared<mysql_session>("localhost", "ormplusplus", "root", "root")},
		{"sqlite", make_shared<sqlite_session>("testdb.sqlite")}
	};
	for(auto& session_entry : db_sessions){
		cout<<"running tests on "<<session_entry.first<<" database"<<endl;
		db::set_default_session(session_entry.second);
		assert_table_creation();
		test_single_insert_and_select();
		cout<<"==================================================="<<endl;
	}
	return 0;
}



