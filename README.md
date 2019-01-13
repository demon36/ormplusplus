# ormplusplus (pre-alpha)
A basic ORM for C++11 that does not precompile your code

Targeting MySQL support, SQLite and others can be supported in the future

### Usage example
```cpp
BOUND_MODEL(Client, "client_info")
{
public:
	DEFINE_ATTRIB(String, name).withDefault("nameless");
	DEFINE_ATTRIB(Integer, age).withDefault(5);
	DEFINE_ATTRIB(Integer, height);
};

int main(int argc, char** argv)
{
	DB::setDefaultSession(make_shared<MySQLSession>("localhost", "ormplusplus", "root", "root"));
	
	if(!DB::tableExists<Client>()){
		DB::createTable<Client>();
	}

	std::vector<Client> allClients = Client::get();
	std::vector<Client> youngClients = Client::where({
		{"age", "<", 45},
		{"name", "=", NullValue},
	}).orderBy({
		{"age", SortDir::Asc},
		{"height", SortDir::Desc},
	}).limit(100).get();
	
	Client c0 = Client::where({"id", 542}).findFirst();
	Client c1;
	c1.name = "myname";
	c1.age = 25;
	c1.save();

	return 0;
}
```
### TODO
- [x] model definition
- [x] table creation
- [x] assert table exists with schema comparison
- [x] DB session abstraction
- [x] put DB supported types in one place
- [x] update tests
- [ ] use logging
- [ ] single instance select
- [ ] multi instance select
- [ ] single instance save
- [ ] single instance delete
- [ ] single instance update
- [ ] save many
- [ ] delete many
- [ ] update many
- [ ] query order by, limit
- [ ] query group by, having
- [ ] get rid of Poco dependency
- [ ] autosave
- [ ] nested where conditions
- [ ] aggregate fns (AVG, COUNT, SUM, MAX, MIN)
- [ ] store attribute names in database in snake_case
- [ ] add support for sqlite
- [ ] add support for mongodb
- [ ] relational models (maybe?)
- [ ] transactions (maybe?)

### build instructions
- install poco libraries
``
sudo apt-get install libpoco-dev
``
- run make

### guidelines
- no using in header files
- target least number of includes in header files
- use tabs for indents
- use enum classes instead of plain enums
- use override, final, const where possible
- favor compile-time checks over runtime
- if there is a logic in a template class that is more than a few lines, move to a template-less base class if possible
