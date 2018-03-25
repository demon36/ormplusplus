#include <iostream>
#include <vector>
#include <map>

using namespace std;

namespace ORMPlusPlus{

	class NullableField{
		bool null = false;
		bool primaryKey = false;
	public:
		NullableField& asKey(){
			primaryKey = true;
			return *this;
		}
	};

	class Integer{
		int* valuePtr;
	public:
		Integer(void* binding){
			binding = new int;
			valuePtr = (int*)binding;
		}

		int& get(){
			return *valuePtr;
		}

		Integer& asPrimaryKey(){
			return *this;
		}
	};

	template<class Derived>
	class Model{
	public:
		virtual string getTableName() = 0;
		static vector<Derived> findMany(){
			vector<Derived> list;
			Derived d;
			list.push_back(d);
			return list;
		}

		map<string, void*> fields;
	
		template<typename T>
		T mapToField(string colName){
			fields[colName] = nullptr;
			T member(fields[colName]);
			return member;
		}

//		void* mapToField(string colName){
//			fields[colName] = nullptr;
//			return fields[colName];
//		}
	};

}
