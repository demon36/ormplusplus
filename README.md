# ormplusplus (pre-alpha)
A basic ORM for C++11 that does not precompile your code

Targeting SQLite & MySQL support, others can be added in the future

[![Build Status](https://travis-ci.org/demon36/ormplusplus.svg?branch=master)](https://travis-ci.org/demon36/ormplusplus)
[![codecov](https://codecov.io/gh/demon36/ormplusplus/branch/master/graph/badge.svg)](https://codecov.io/gh/demon36/ormplusplus)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

### Usage example -not fully working yet- 
```cpp
BOUND_MODEL(client, "client_info")
{
public:
	DEFINE_ATTRIB(db_string, name).with_default("nameless");
	DEFINE_ATTRIB(db_int, age).with_default(5);
	DEFINE_ATTRIB(db_int, height);
};

int main(int argc, char** argv)
{
	db::set_default_session(make_shared<mysql_session>("localhost", "ormplusplus", "root", "root"));
	
	if(!db::table_exists<client>()){
		db::create_table<client>();
	}

	std::vector<client> all_clients = client::get();
	std::vector<client> young_clients = client::where({
		{"age", "<", 45},
		{"name", "=", null_value},
	}).orderBy({
		{"age", sort_dir::asc},
		{"height", sort_dir::desc},
	}).limit(100).get();
	
	client c0 = client::where({"id", 542}).find_first();
	client c1;
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
- [x] get rid of Poco dependency
- [x] add basic support for sqlite
- [ ] select, insert, update, delete one
- [ ] select, insert, update, delete many
- [ ] order by, limit, group by, having
- [ ] insert or update if exists
- [ ] nested where conditions
- [ ] aggregate fns (AVG, COUNT, SUM, MAX, MIN)
- [ ] separate sqlite and mysql compile time dependencies
- [ ] autosave
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
