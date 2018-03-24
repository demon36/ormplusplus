#include <iostream>
#include <vector>

using namespace std;

template<class Derived>
class Model
{
public:
	static vector<Derived> findMany(){
//		vector<Derived*> list;
//		Derived* d = new Derived();
//		Entity* e = static_cast<Entity*>(d);
//		e->numCols = 14;
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
	int& mapToField(string fieldName){
		return fieldvalue;
	}
};
