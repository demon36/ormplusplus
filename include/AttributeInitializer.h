#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include <string>
#include <type_traits>
#include <exception>

#include "TableColumn.h"

namespace ORMPlusPlus{

class NullableFieldBase;
template<class UserModel>
class Model;

template<class UserModel, class AttribType>
class AttributeInitializer{
protected:
	AttribType* fieldPtr;
	Model<UserModel>* modelInstance;
public:
	AttributeInitializer(AttributeInitializer& that){
		this->fieldPtr = that.fieldPtr;
		this->modelInstance = that.modelInstance;
	}

	AttributeInitializer(Model<UserModel>* modelInstance, std::string attributeName){
		this->modelInstance = modelInstance;
		Model<UserModel>::template addColumnIfNotExists<AttribType>(attributeName);
		fieldPtr = modelInstance->template addAttributeVariable<AttribType>(attributeName);
	}

	AttributeInitializer& withDefault(AttribType value){
		*fieldPtr = value;
		return *this;
	}

	//fns that might be needed
	AttributeInitializer& asPrimary();//change columnDefs
	AttributeInitializer& asIndex();//change columnDefs, does it apply on numerical types only?


	~AttributeInitializer(){
	}

	operator AttribType&(){
		return *fieldPtr;
	}
};

}
#endif /* INCLUDE_ATTRIBUTEINITIALIZER_H_ */
