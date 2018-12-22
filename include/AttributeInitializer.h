#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include <string>
#include <type_traits>
#include <exception>

#include "ModelBase.h"
#include "TableColumn.h"

namespace ORMPlusPlus{

class NullableFieldBase;

template<class AttribType>
class AttributeInitializer{
protected:
	AttribType* fieldPtr;
	ModelBase* modelInstance;
public:
	AttributeInitializer(AttributeInitializer& that){
		this->fieldPtr = that.fieldPtr;
		this->modelInstance = that.modelInstance;
	}

	AttributeInitializer(AttribType* attribVariablePtr, std::string attributeName){
		this->modelInstance = modelInstance;
		fieldPtr = attribVariablePtr;
	}

	AttributeInitializer& withDefault(AttribType value){
		*fieldPtr = value;
		return *this;
	}

	//fns that might be needed

	//TODO: make sure there is only one primary
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
