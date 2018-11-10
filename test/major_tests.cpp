#include <iostream>
#include "ormplusplus.h"

using namespace std;
using namespace ORMPlusPlus;

#define TEST(expression, expectedValue) printTestResult(string("test ")+#expression, expression, expectedValue)

void printTestResult(string expression, bool result, bool expectedValue){
	if(result == expectedValue){
		cout<<expression<<" succeeded"<<endl;
	}else{
		cerr<<expression<<" failed"<<endl;
	}
}

BOUND_MODEL(Client)
{
public:
	static string getTableName(){
		return "client_info";
	}

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
	Client c;
	TEST(c.name.get() == "nameless", true);
	TEST(c.age.get() == 5, true);
	TEST(c.height.get() == 4, false);
	TEST(Client::columnExists("name"), true);
	TEST(Client::columnExists("age"), true);
	
	DB::initialize("localhost", "ormplusplus", "root", "root");

	TEST(assertClassIsUserModel<Client>(), true);
	TEST(assertClassIsUserModel<Integer>(), false);
	TEST(DB::tableExists<Client>(), false);
	DB::createTable<Client>();
	TEST(DB::tableExists<Client>(), true);
	return 0;
}



