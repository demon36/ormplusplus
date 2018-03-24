#include <iostream>

#include "ORMPlusPlus.h"

using namespace std;

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

#define MAP_TO_FIELD()


class Client : public Model<Client>
{
public:
	string getTableName(){
		return "";
	}
	
	int x;
	int& y = mapToField("id");
};


struct Person
{
    std::string name;
    std::string address;
    int         age;
};

void testConnection(){
	Poco::Data::MySQL::Connector::registerConnector();
    Session session("MySQL", "host=localhost;port=3306;db=ormplusplus;user=root;password=root;compress=true;auto-reconnect=true");
    session << "DROP TABLE IF EXISTS Person", now;
    session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR(20), Age INTEGER(3))", now;
    Person person = 
    {
        "Bart Simpson",
        "Springfield",
        12
    };
    Statement insert(session);
    insert << "INSERT INTO Person VALUES(?, ?, ?)",
        use(person.name),
        use(person.address),
        use(person.age);
    insert.execute();
    person.name    = "Lisa Simpson";
    person.address = "Springfield";
    person.age     = 10;
    insert.execute();
    Statement select(session);
    select << "SELECT Name, Address, Age FROM Person",
        into(person.name),
        into(person.address),
        into(person.age),
        range(0, 1); //  iterate over result set one row at a time
    while (!select.done())
    {
        select.execute();
        std::cout << person.name << " " << person.address << " " << person.age << std::endl;
    }
}

int main(int argc, char** argv)
{
	std::vector<Client> list = Client::findMany();
	int s = list.size();
	int numcols = list[0].numCols;
	cout<<numcols<<endl;
	
	Client c;
	cout<<c.y<<endl;
    
    return 0;
}
