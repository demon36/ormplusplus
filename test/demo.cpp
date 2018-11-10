#include <iostream>

#include "ormplusplus.h"

using namespace std;
using namespace ORMPlusPlus;

#define TEMP_DEFINE_ATTR(TYPE, NAME) TYPE NAME = initialize2<TYPE>(this)->withColumnName(#NAME);

BOUND_MODEL(Client)
{
public:
	static string getTableName(){
		return "clieny_info";
	}

//	DEFINE_ATTR(Integer, id).asPrimaryKey().withDefault(8).withColumnName("ID");
//	DEFINE_ATTR(Integer, age).withDefault(0);
//	DEFINE_ATTR(String, name).withDefault("test");
//	String& lastName = mapToField<String>("lastName").withDefault("_");
//	String lastName = initializeAttrib()->withColumnName<String>("colname");
	String lastName = initializeAttrib()->withColumn<String>("colname");
	Integer age = initializeAttrib()->withColumn<Integer>("age");
//	String lastName = AttributeInitializer<Client,String>(this).withColumnName("colname");
//	TEMP_DEFINE_ATTR(String, firstname);
};

int main(int argc, char** argv)
{
	Client c;
	
	cout<<"Client::columnDefs.size()="<<Client::columnDefs.size()<<endl;
	for(auto& column : Client::columnDefs){
		cout<<column.second.getName()<<endl;
	}

	try{
		DB::initialize("localhost", "ormplusplus", "root", "root");
//		DB::createOrReplaceTable(Client);
		std::vector<Client> allClients = Client::get();
		std::vector<Client> youngClients = Client::where({
			{"age", "=", 45}, //todo: should this be accepted?
			{"name", "=", NullValue},
		}).get();

		Client c;
//		cout<<c.id.get()<<endl;
//		cout<<c.age.get()<<endl;
//		c.age = 3;
//		cout<<c.age.get()<<endl;
	}catch(Poco::Exception& ex){
		cout<<ex.message()<<endl;
	}
    return 0;
}



