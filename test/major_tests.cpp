#include "ormplusplus.h"

#include <iomanip>

using namespace std;
using namespace ORMPlusPlus;

#define ASSERT(expression) printTestResult(#expression, expression)

void printTestResult(const string& expression, bool result){
	ostream& targetStream = result ? cout : cerr ;
	targetStream << "assertion `" << left << setw(60) << expression + "` ";
	targetStream << " >>>>>>> " << right << setw(20) << (result ? "succeeded" : "failed") << endl;
}

BOUND_MODEL(Client, "client_info")
{
public:
	DEFINE_ATTRIB(Integer, id).autoIncrement().asPrimary();
	DEFINE_ATTRIB(String, name).withDefault("nameless");
	DEFINE_ATTRIB(Integer, age).withDefault(5).asNullable();
	DEFINE_ATTRIB(Integer, height);
//	DEFINE_ATTRIB(DateTime, dob).withDefault(3);
//	Integer height = initializeAttrib<Integer>("height");
};

void testModelDefinition(){
	ASSERT(DB::isUserModelClass<Client>());
	ASSERT(!DB::isUserModelClass<Integer>());

	Client c;
	ASSERT(c.id.isNull());
	ASSERT(c.name == "nameless");
	ASSERT(c.age == 5);
	ASSERT(c.height.isNull());

	const TableSchema& schema = Client::getSchema();
	ASSERT(schema.at("id").isAutoIncrement());
	ASSERT(schema.at("id").isPrimary());
	ASSERT(schema.at("name").getDefaultValue() == "nameless");
	ASSERT(schema.at("name").isText());
	ASSERT(!schema.at("name").isIntegral());
	ASSERT(schema.at("age").getDefaultValue() == "5");
	ASSERT(schema.at("age").isNullable());
	ASSERT(!schema.at("height").isAutoIncrement());
	ASSERT(!schema.at("height").isNullable());
	ASSERT(schema.at("height").isIntegral());
	ASSERT(schema.at("height").getDefaultValue().empty());
	ASSERT(!schema.at("height").isPrimary());
	ASSERT(!schema.at("height").isText());
}

void assertTableCreation(){
	DB::dropTable<Client>();
	ASSERT(!DB::tableExists<Client>());
	DB::createTable<Client>();
	ASSERT(DB::tableExists<Client>());
	ASSERT(DB::tableExists<Client>(true));
}

void testInsertAndSelect(){
	//TODO: test with datetime
	vector<Client> c0 = Client::where({
		{"id", "=", 542}
	}).select();
}

int main(int argc, char** argv)
{
	try{
		DB::setDefaultSession(make_shared<MySQLSession>("localhost", "ormplusplus", "root", "root"));
		testModelDefinition();
		assertTableCreation();
		testInsertAndSelect();
	}catch(Poco::Exception& ex){
		cerr<<ex.displayText()<<endl;
	}catch(exception& ex){
		cerr<<ex.what()<<endl;
	}
	return 0;
}



