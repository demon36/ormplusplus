#include "ormplusplus.h"

using namespace std;
using namespace ORMPlusPlus;

#define TEST(expression, expectedValue) printTestResult(#expression ,expression, expectedValue)

void printTestResult(string expression, bool result, bool expectedValue){
	string temp = "expression `"+expression+"` yielded "+ (result ? "true" : "false");
	if(result == expectedValue){
		cout<<temp<<" ---------------- success"<<endl;
	}else{
		cerr<<temp<<" ---------------- failure"<<endl;
	}
}

BOUND_MODEL(Client, "client_info")
{
public:
	DEFINE_ATTRIB(Integer, ID).autoIncrement();
	DEFINE_ATTRIB(String, name).withDefault("nameless");//TODO: assert "nameless" gets set correctly
	DEFINE_ATTRIB(Integer, age).withDefault(5);
	DEFINE_ATTRIB(Integer, height);
//	Integer height = initializeAttrib<Integer>("height");
};

template<class U>
bool assertClassIsUserModel(){
	try{
		DB::assertClassIsUserModel<U>();
		return true;
	}catch(std::runtime_error& err){
		return false;
	}
}

int main(int argc, char** argv)
{
	try{
		Client c;
		TEST(c.name == "nameless", true);
		TEST(c.age == 5, true);
		TEST(c.height == 4, false);
		TEST(Client::columnExists("name"), true);
		TEST(Client::columnExists("age"), true);
		DB::setDefaultSession(make_shared<MySQLSession>("localhost", "ormplusplus", "root", "root"));
		TEST(assertClassIsUserModel<Client>(), true);
		TEST(assertClassIsUserModel<Integer>(), false);
		DB::dropTable<Client>();
		TEST(DB::tableExists<Client>(), false);
		DB::createTable<Client>();
		TEST(DB::tableExists<Client>(), true);
		TEST(DB::tableExists<Client>(true), true);
	}catch(Poco::Exception& ex){
		cerr<<ex.displayText()<<endl;
	}
	return 0;
}



