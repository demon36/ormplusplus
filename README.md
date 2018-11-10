# ormplusplus
A basic ORM for C++11 that does not precompile your code

Currently MySQL only is supported but the ORM is based on Poco libraries so it should not be hard to add support for SQLite, Redis or Mongodb.

### Usage example
```cpp
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

int main(int argc, char** argv)
{
	DB::initialize("localhost", "ormplusplus", "root", "root");
	Client c;
	if(!DB::tableExists<Client>()){
		DB::createTable<Client>();
	}
	std::vector<Client> allClients = Client::get();
	std::vector<Client> youngClients = Client::where({
		{"age", "<", 45},
		{"name", "=", NullValue},
	}).get();
	
	return 0;
}
}
```
### Implemented features
- defining a model
### todo features
- create table
- assert table exists with schema comparison
- single instance save/load/delete
- model load many with conditions
- model delete many, update many
- relational models (maybe?)

#### plan:
- use CRTP pattern for returning instances of child class from parent class (findMany, removeMany, ..)
- use macros to facilitate user model definitions
- use decorator pattern for conditioned queries:  `model::where({loc_id, 20})->orderBy({id, age})->limit(100)->get();`
- use camelCase for model user defined members and snake_case for column bindings
- use nullable data types

### build instructions
- install poco libraries
``
sudo apt-get install libpoco-dev
``
- run make
