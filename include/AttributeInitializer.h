#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include "AttributeInitializerBase.h"

namespace ORMPlusPlus{

template<class AttribType>
class AttributeInitializer : public AttributeInitializerBase{
private:
	AttribType nullableField;
public:
	AttributeInitializer(AttributeInitializer& that)
	: AttributeInitializerBase(that), nullableField(that.nullableField)
	{
	}

	AttributeInitializer(NullableFieldBase& NFBase, ModelBase& modelInstance, const std::string& attributeName)
	: AttributeInitializerBase(NFBase, modelInstance, attributeName), nullableField(NFBase)
	{
	}

	AttributeInitializer& withDefault(AttribType value){
		//TODO: check operator= is implemented correctly
		nullableField = value;
		return *this;
	}

	//TODO: make sure there is only one primary -maybe?-
	AttributeInitializer& asPrimary();//change columnDefs
	AttributeInitializer& asIndex();//change columnDefs, does it apply on numerical types only?

	~AttributeInitializer(){
	}

//	operator AttribType&&(){
//		//FIXME: which recipient ctor will be called here ?
//		return std::move(nullableField);
//	}
};

}
#endif /* INCLUDE_ATTRIBUTEINITIALIZER_H_ */
