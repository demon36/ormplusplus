# ormplusplus (pre-alpha)
A basic ORM for C++11 that does not precompile your code

Targeting SQLite & MySQL support, others can be added in the future

[![Build Status](https://travis-ci.org/demon36/ormplusplus.svg?branch=master)](https://travis-ci.org/demon36/ormplusplus)
[![codecov](https://codecov.io/gh/demon36/ormplusplus/branch/master/graph/badge.svg)](https://codecov.io/gh/demon36/ormplusplus)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

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
- [x] use logging
- [x] add basic support for sqlite
- [ ] insert one
- [ ] select one
- [ ] insert many
- [ ] select many
- [ ] delete one
- [ ] update one
- [ ] delete many
- [ ] update many
- [ ] query order by, limit
- [ ] query group by, having
- [x] get rid of Poco dependency
- [ ] autosave
- [ ] nested where conditions
- [ ] aggregate fns (AVG, COUNT, SUM, MAX, MIN)
- [ ] insert or update if exists
- [ ] separate sqlite and mysql dependencies
- [ ] add support for mongodb
- [ ] check thread safety
- [ ] relational models (maybe?)
- [ ] transactions (maybe?)

### build instructions
- install mysql and sqlite c connectors
``
sudo apt-get install default-libmysqlclient-dev libsqlite3-dev #on debian bullseye
``
- run make

### guidelines
- no `using` in header files
- target least number of includes in header files
- use tabs for indents
- use enum classes instead of plain enums
- use override, final, const& where possible
- favor compile-time checks over runtime
- if there is a logic in a template class that is more than a few lines, move to a template-less base class if possible
- disable implicit casting via `-Wconversion -Werror`
- either mark as final or define a virtual destructor
