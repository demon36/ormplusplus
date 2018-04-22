#include <iostream>

#include "ORMPlusPlus.h"

using namespace std;
using namespace ORMPlusPlus;

class Client : public ORMPlusPlus::Model<Client>
{
public:
	string getTableName(){
		return "Person";
	}
	
	DEFINE_ATTR(Integer, id).asPrimaryKey().withDefault(8).withColumnName("ID");
	DEFINE_ATTR(Integer, age).withDefault(0);
};

int main(int argc, char** argv)
{
	ORMPlusPlus::DB::initialize("localhost", "ormplusplus", "root", "root");
	vector<Client> allClients = Client::get();
	vector<Client> youngClients = Client::where({
		{COLUMN(age), "=", ""}
	}).get();

	Client c;
	cout<<c.id.get()<<endl;
	cout<<c.age.get()<<endl;
	c.age = 3;
	cout<<c.age.get()<<endl;

    return 0;
}
