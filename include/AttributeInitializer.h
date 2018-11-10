#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include <string>

#include "TableColumn.h"

namespace ORMPlusPlus{

class NullableFieldBase;
template<class UserModel>
class Model;

template<class UserModel>
class AttributeInitializer{
	NullableFieldBase* fieldPtr;
	Model<UserModel>* modelInstance;
public:
	std::string test;
	AttributeInitializer(Model<UserModel>* modelInstance){
		this->modelInstance = modelInstance;
		std::cerr<<"AttributeInitializer::AttributeInitializer()"<<std::endl;
		test = "live";
	}

    template<class NullableType>
    AttributeInitializer& withColumn(std::string attributeName){
		//TODO: datatype hardcoded for now, change later
		Model<UserModel>::columnDefs[attributeName] = TableColumn(DataType::_String, attributeName);
		modelInstance->attributes[attributeName] = new NullableType();
		fieldPtr = modelInstance->attributes[attributeName];
		return *this;
    }

	NullableFieldBase* getRef(){
		return fieldPtr;
	}
	
	~AttributeInitializer(){
		std::cerr<<"AttributeInitializer::~AttributeInitializer()"<<std::endl;
	}

};

}
#endif /* INCLUDE_ATTRIBUTEINITIALIZER_H_ */
