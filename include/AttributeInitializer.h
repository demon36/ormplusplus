#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include <string>

#include "ModelBase.h"
#include "TableColumn.h"

namespace ORMPlusPlus{

template<class UserModel, class NullableType>
class AttributeInitializer{
	NullableType* fieldPtr;
	UserModel* modelInstance;
public:
	AttributeInitializer(UserModel* modelInstance){
		this->modelInstance = modelInstance;
	}

	AttributeInitializer& withColumnName(std::string attributeName){
		UserModel::columns["attributeName"] = TableColumn();
//		UserModel::columns["attributeName"].setType(NullableType::getTypeName());
//		addColumn(attributeName, FieldType::getTypeName());
		this->modelInstance->fieldValues[attributeName] = new NullableType(modelInstance, attributeName);
//		return static_cast<FieldType&>(*fieldValues[attributeName]);
	}

	NullableType& getRef(){
		return *fieldPtr;
	}

};

}
#endif /* INCLUDE_ATTRIBUTEINITIALIZER_H_ */
