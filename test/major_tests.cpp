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
	DEFINE_ATTRIB(Integer, id).autoIncrement();
	DEFINE_ATTRIB(String, name).withDefault("nameless");
	DEFINE_ATTRIB(Integer, age).withDefault(5);
	DEFINE_ATTRIB(Integer, height);
//	DEFINE_ATTRIB(DateTime, dob).withDefault(3);
//	Integer height = initializeAttrib<Integer>("height");
};

void testModelDefinition(){
	Client c;
	ASSERT(c.name == "nameless");
	ASSERT(c.age == 5);
	ASSERT(c.height != 4);
	ASSERT(Client::columnExists("name"));
	ASSERT(Client::columnExists("age"));
	ASSERT(DB::isUserModelClass<Client>());
	ASSERT(DB::isUserModelClass<Integer>() == false);
	DB::dropTable<Client>();
	ASSERT(DB::tableExists<Client>() == false);
	DB::createTable<Client>();
	ASSERT(DB::tableExists<Client>());
	ASSERT(DB::tableExists<Client>(true));
}

void testInsertAndSelect(){
	//TODO: test with datetime
	vector<Client> c0 = Client::where({
		{"id", "=", 542}
	}).select();

	if(!c0.empty()){
		cerr<<Poco::format("\tid:%s\n\tname:%s\n\tage:%s\n\theight:%s\n\t",
				c0[0].id.toString(),
				c0[0].name.toString(),
				c0[0].age.toString(),
				c0[0].height.toString()
				);
	}
}

int main(int argc, char** argv)
{
	try{
		DB::setDefaultSession(make_shared<MySQLSession>("localhost", "ormplusplus", "root", "root"));
		testModelDefinition();
		testInsertAndSelect();
	}catch(Poco::Exception& ex){
		cerr<<ex.displayText()<<endl;
	}catch(exception& ex){
		cerr<<ex.what()<<endl;
	}
	return 0;
}



