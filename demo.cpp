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
	
	DEFINE_MEMBER(Integer, id).asPrimaryKey().withDefault(8).withColumnName("ID");
};

int main(int argc, char** argv)
{
	ORMPlusPlus::DB::initialize("localhost", "ormplusplus", "root", "root");
	Client c;
	cout<<c.id.get()<<endl;
	c.id.get() = 3;
	cout<<c.id.get()<<endl;
	for(int i = 0; i < c.getColumnNames().size(); i++){
		cout<<"column["<<i<<"]="<<c.getColumnNames()[i]<<endl;
	}

    return 0;
}
