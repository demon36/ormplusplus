#include <iostream>
#include "ormplusplus.h"

using namespace std;
using namespace ORMPlusPlus;

#define TEST(expression) printTestResult(#expression, expression)

void printTestResult(string expression, bool result){
	if(result){
		cout<<expression<<" succeeded"<<endl;
	}else{
		cerr<<expression<<" failed"<<endl;
	}
}

#define CLIENT_TABLE "client_info"
#define NAME_ATTR_COLUMN "name"
#define AGE_ATTR_COLUMN "age"

BOUND_MODEL(Client)
{
public:
	static string getTableName(){
		return CLIENT_TABLE;
	}

	String name = initializeAttrib()->withColumn<String>(NAME_ATTR_COLUMN);
	Integer age = initializeAttrib()->withColumn<Integer>(AGE_ATTR_COLUMN);
};

bool assertDefinedColumnsExist(string columnName){
	if(Client::columnDefs.find(columnName) == Client::columnDefs.end()){
		return false;
	}else{
		return true;
	}
}

int main(int argc, char** argv)
{
	Client c;
	TEST(assertDefinedColumnsExist(NAME_ATTR_COLUMN));
	TEST(assertDefinedColumnsExist(AGE_ATTR_COLUMN));
	
    return 0;
}



