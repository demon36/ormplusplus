#include <iostream>
#include "ormplusplus.h"

using namespace std;
using namespace ORMPlusPlus;

#define TEST(expression, expectedValue) printTestResult(string("expression \"")+#expression+"\" yielded "+(expectedValue?"true":"false"), expression, expectedValue)

void printTestResult(string expression, bool result, bool expectedValue){
	if(result == expectedValue){
		cout<<expression<<" ---------------- success"<<endl;
	}else{
		cerr<<expression<<" ---------------- failure"<<endl;
	}
}


//TODO: send table name as template parameter
BOUND_MODEL(Client, "client_info")
{
public:
	DEFINE_ATTRIB(String, name).withDefault(string("nameless"));
	DEFINE_ATTRIB(Integer, age).withDefault(5);
	DEFINE_ATTRIB(Integer, height);
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
		TEST(c.name.get() == "nameless", true);
		TEST(c.age.get() == 5, true);
		TEST(c.height.get() == 4, false);
		TEST(Client::columnExists("name"), true);
		TEST(Client::columnExists("age"), true);
		DB::initialize("localhost", "ormplusplus", "root", "root");
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



