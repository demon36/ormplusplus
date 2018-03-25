#include <iostream>
#include <vector>
#include <map>

using namespace std;

namespace ORMPlusPlus{

	class Integer{
	public:
		ORMPlusPlus::Integer& operator=(const ORMPlusPlus::Integer&) = delete;

		Integer(int* binding):
		value(binding)
		{
		}

		Integer& asKey(){
			primaryKey = true;
			return *this;
		}

		operator int&() { return *value; }

		int* value;
		bool null = false;
		bool primaryKey = false;
	};

	class Table{
		map<string, void*> fields;
	public:
		template<typename T>
		T& mapToField(string columnName){
			fields[columnName] = new T;
			return (T&)fields[columnName];
		}
	};

	template<class Derived>
	class Model{
	public:
		static vector<Derived> findMany(){
			vector<Derived> list;
			Derived d;
			d.numCols = 14;
			list.push_back(d);
			return list;
		}

		int numCols = 0;
		virtual string getTableName() = 0;
		int fieldvalue = 2;
	
		template<typename T>
		int& mapToField(string colName){
			return fieldvalue;
		}

		Integer mapNullable(string colName){
//			Integer n(this->mapToField<int>(colName));
			Integer n(&fieldvalue);
		}
	};

}
