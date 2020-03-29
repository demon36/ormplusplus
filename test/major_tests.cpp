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

struct client : public model<client>
{
	db_long id = initialize_attrib("id").auto_increment().as_primary();
	db_string name = initialize_attrib("name").with_default("nameless");
	db_int age = initialize_attrib("age").with_default("5").as_nullable();
	db_int height = initialize_attrib("height");
	db_datetime dob = initialize_attrib("dob").as_nullable(false);
};

struct mini_bus : public model<mini_bus>
{
	db_int id = initialize_attrib("id").auto_increment().as_primary();
	db_string model = initialize_attrib("model").with_default("my_van");
	constexpr static const char* table_name = "minibuses";
};


void test_model_definition(){
	ASSERT(mini_bus::get_table_name() == "minibuses");
	ASSERT(client::get_table_name() == "client");
	ASSERT(model_base::is_user_model_class<client>());
	ASSERT(!model_base::is_user_model_class<db_int>());

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

void assert_table_creation(db_session_base& dbsession){
	dbsession.drop_table<client>();
	ASSERT(!dbsession.table_exists<client>());
	dbsession.create_table<client>();
	ASSERT(dbsession.table_exists<client>());
	ASSERT(dbsession.table_exists<client>(true));
}

void test_single_insert_and_select(db_session_base& dbsession){
	client c0;
	attribs_map& temp = c0.get_attribs();
	temp.size();
	c0.age = 15;
	c0.name = "robert";
	c0.height = 167;
	::tm dobtm;
	strptime("1993-09-30 17:20:21", "%Y-%m-%d %H:%M:%S", &dobtm);
	c0.dob = dobtm;
	dbsession.insert(c0, true);

	client c1 = dbsession.execute(client::select_one().where({"id", "=", c0.id}));

	ASSERT(c0.equals(c1));
}

void test_multi_insert_and_select(){
	std::vector<client> clients(10);
}

int main(int argc, char** argv)
{
	logger::set_level(logger::lvl::DBUG);
	test_model_definition();
	unique_ptr<db_session_base> db_sessions[] = {
			unique_ptr<db_session_base>(new mysql_session("localhost", "ormplusplus", "root", "root")),
			unique_ptr<db_session_base>(new sqlite_session("testdb.sqlite"))
	};
	for(auto& session : db_sessions){
		cout<<"running tests on "<<ormplusplus::type_info::get_type_name(typeid(*session))<<" database"<<endl;
		assert_table_creation(*session);
		test_single_insert_and_select(*session);
		cout<<"==================================================="<<endl;
	}
	return 0;
}
