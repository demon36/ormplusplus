# ormplusplus (pre-alpha)
A basic ORM for C++11 that does not precompile your code

Targeting SQLite & MySQL support, others can be added in the future

[![Build Status](https://travis-ci.org/demon36/ormplusplus.svg?branch=master)](https://travis-ci.org/demon36/ormplusplus)
[![codecov](https://codecov.io/gh/demon36/ormplusplus/branch/master/graph/badge.svg)](https://codecov.io/gh/demon36/ormplusplus)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

### Usage example -not fully working yet- 
```cpp
struct client : public model<client>
{
	db_long id = initialize_attrib("id").auto_increment().as_primary();
	db_string name = initialize_attrib("name").with_default("nameless");
	db_int age = initialize_attrib("age").with_default("5").as_nullable();
	db_int height = initialize_attrib("height");
	db_datetime dob = initialize_attrib("dob").as_nullable(false);

	// constexpr static const char* table_name = "clients";//optional override for actual table name, default is class name
};

int main(int argc, char** argv)
{
	sqlite_session sqlite("testdb.sqlite");
	
	if(!sqlite::table_exists<client>()){
		sqlite::create_table<client>();
	}

	std::vector<client> all_clients = sqlite.exec(client::select_many().where());
	std::vector<client> young_clients = sqlite.exec(client::select_many().where({
		{"age", "<", 45},
		{"name", "=", null_value},
	}).orderBy({
		{"age", sort_dir::asc},
		{"height", sort_dir::desc},
	}).limit(100));
	
	client c0 = sqlite.exec(client::select_one().where({"id", 542}));
	client c1;
	c1.name = "myname";
	c1.age = 25;
	sqlite.insert(c1);

	sqlite.exec(client::delete().where({"id", "=", 34}));
}
```

### implemented features:
- [x] schema definition
- [x] model binding
- [x] table creation & deletion
- [x] check table exists with schema comparison
- [x] logging
- [x] basic support for mysql & sqlite
- [ ] single row CRUD
- [ ] multi row CRUD
- [ ] order by, limit
- [ ] insert or update if exists
- [ ] separate sqlite and mysql compile time dependencies
- [ ] nested where conditions
- [ ] transactions
- [ ] relational models (maybe?)
- [ ] aggregate fns (AVG, COUNT, SUM, MAX, MIN)
- [ ] group by, having
- [ ] thread safety ?

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
