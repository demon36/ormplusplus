#include <iostream>

#include "ORMPlusPlus.h"

using namespace std;
using namespace ORMPlusPlus;

class Client : public ORMPlusPlus::Model<Client>
{
public:
	string getTableName(){
		return "";
	}
	
	DEFINE_MEMBER(Integer, test).asPrimaryKey().withDefault(8);
};

int main(int argc, char** argv)
{
	Client c;
	cout<<c.test.get()<<endl;
	c.test.get() = 3;
	cout<<c.test.get()<<endl;
	cout<<c.test.get()<<endl;

    return 0;
}
