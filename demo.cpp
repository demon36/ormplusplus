#include <iostream>

#include "ORMPlusPlus.h"

using namespace std;

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

#define DEFINE_MEMBER(DATATYPE, NAME) DATATYPE& NAME = mapToField<DATATYPE>(#NAME)
#define DEFINE_MEMBER_WITH_COL_NAME(DATATYPE, NAME, COL_NAME) DATATYPE& NAME = mapToField<DATATYPE>(COL_NAME)
#define DEFINE_MEMBER2(DATATYPE, NAME) DATATYPE& NAME = mapNullable(#NAME)

class Client : public ORMPlusPlus::Model<Client>
{
public:
	string getTableName(){
		return "";
	}
	
	DEFINE_MEMBER(int, y);
	DEFINE_MEMBER_WITH_COL_NAME(int, id, "USER_ID");
	ORMPlusPlus::Integer test = mapNullable("loc_id").asKey();
	DEFINE_MEMBER2(ORMPlusPlus::Integer, test2).asKey();
};

class M1;
int testFn(M1* m){
	return 1;
}

class M1{
	M1():
	x(testFn(this))
	{

	}
	int x;
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
	cout<<c.test2<<endl;
	cout<<c.test2<<endl;
    
    return 0;
}
