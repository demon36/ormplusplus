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

class Client : public Model<Client>
{
public:
	DEFINE_ATTRIB(Integer, id).autoIncrement().asPrimary();
	DEFINE_ATTRIB(String, name).withDefault("nameless");
	DEFINE_ATTRIB(Integer, age).withDefault(5).asNullable();
	DEFINE_ATTRIB(Integer, height);
	DEFINE_ATTRIB(DateTime, dob).asNullable(false);

	constexpr static const char* _id = "asdf"; //use instead of 2nd template parameter ??
};

class MiniBus : public Model<MiniBus, _("minibuses")>
{
public:
	DEFINE_ATTRIB(Integer, id).autoIncrement().asPrimary();
	DEFINE_ATTRIB(String, model).withDefault("myVan");
};


void testModelDefinition(){
	ASSERT(MiniBus::getTableName() == "minibuses");
	ASSERT(Client::getTableName() == "Client");
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

	ASSERT(c0.equals(c1));
}

void testMultiInsertAndSelect(){
	std::vector<Client> clients(10);
}

int main(int argc, char** argv)
{
	Logger::setLevel(Logger::Lv::DBUG);
	try{
		DB::setDefaultSession(make_shared<MySQLSession>("localhost", "ormplusplus", "root", "root"));
		testModelDefinition();
		assertTableCreation();
		testSingleInsertAndSelect();
	}catch(exception& ex){
		cerr<<ex.what()<<endl;
	}
	return 0;
}



