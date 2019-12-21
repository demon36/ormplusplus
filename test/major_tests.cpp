#include <logger.h>
#include "ormplusplus.h"
#include <iomanip>

using namespace std;
using namespace ormplusplus;

#define ASSERT(expression) printTestResult(#expression, expression)

void printTestResult(const string& expression, bool result){
	ostream& targetStream = result ? cout : cerr ;
	targetStream << "assertion `" << left << setw(60) << expression + "` ";
	targetStream << " >>>>>>> " << right << setw(20) << (result ? "succeeded" : "failed") << endl;
}

class Client : public model<Client>
{
public:
	DEFINE_ATTRIB(Long, id).autoIncrement().asPrimary();
	DEFINE_ATTRIB(String, name).withDefault("nameless");
	DEFINE_ATTRIB(Integer, age).withDefault(5).asNullable();
	DEFINE_ATTRIB(Integer, height);
	DEFINE_ATTRIB(DateTime, dob).asNullable(false);

	constexpr static const char* _id = "asdf"; //use instead of 2nd template parameter ??
};

class MiniBus : public model<MiniBus, _("minibuses")>
{
public:
	DEFINE_ATTRIB(Integer, id).autoIncrement().asPrimary();
	DEFINE_ATTRIB(String, model).withDefault("myVan");
};


void testModelDefinition(){
	ASSERT(MiniBus::getTableName() == "minibuses");
	ASSERT(Client::getTableName() == "Client");
	ASSERT(db::isUserModelClass<Client>());
	ASSERT(!db::isUserModelClass<Integer>());

	Client c;
	ASSERT(c.id.isNull());
	ASSERT(c.name == "nameless");
	ASSERT(c.age == 5);
	ASSERT(c.age < 6);
	ASSERT(c.height.isNull());

	const table_schema& schema = Client::getSchema();
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

void assertTableCreation(){
	db::dropTable<Client>();
	ASSERT(!db::tableExists<Client>());
	db::createTable<Client>();
	ASSERT(db::tableExists<Client>());
	ASSERT(db::tableExists<Client>(true));
}

void testSingleInsertAndSelect(){
	Client c0;
	c0.age = 15;
	c0.name = "robert";
	c0.height = 167;
	::tm dobtm;
	strptime("1993-09-30 17:20:21", "%Y-%m-%d %H:%M:%S", &dobtm);
	c0.dob = dobtm;
	c0.insert(true);

	Client c1 = Client::where({
		{"id", "=", c0.id}
	}).selectOne();

	ASSERT(c0.equals(c1));//todo: fix this
}

void testMultiInsertAndSelect(){
	std::vector<Client> clients(10);
}

int main(int argc, char** argv)
{
	logger::set_level(logger::lvl::DBUG);
	testModelDefinition();
	map<string, shared_ptr<db_session_base>> dbSessions = {
		{"mysql", make_shared<mysql_session>("localhost", "ormplusplus", "root", "root")},
		{"sqlite", make_shared<sqlite_session>("testdb.sqlite")}
	};
	for(auto& sessionEntry : dbSessions){
		cout<<"running tests on "<<sessionEntry.first<<" database"<<endl;
		db::set_default_session(sessionEntry.second);
		assertTableCreation();
		testSingleInsertAndSelect();
		cout<<"==================================================="<<endl;
	}
	return 0;
}



