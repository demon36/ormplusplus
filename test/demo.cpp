#include <iostream>

#include "ormplusplus.h"

using namespace std;
using namespace ORMPlusPlus;

BOUND_MODEL(Client)
{
public:
	static string getTableName(){
		return "Person";
	}

	DEFINE_ATTR(Integer, id).asPrimaryKey().withDefault(8).withColumnName("ID");
	DEFINE_ATTR(Integer, age).withDefault(0);
	DEFINE_ATTR(String, name).withDefault("test");
	String& lastName = mapToField<String>("lastName").withDefault("_");
};

int main(int argc, char** argv)
{
	DB::initialize("localhost", "ormplusplus", "root", "root");
	std::vector<Client> allClients = Client::get();
	std::vector<Client> youngClients = Client::where({
		{"age", "=", ""}, //todo: should this be accepted
		{"name", "=", NullValue},
	}).get();

	Client c;
	cout<<c.id.get()<<endl;
	cout<<c.age.get()<<endl;
	c.age = 3;
	cout<<c.age.get()<<endl;

    return 0;
}



