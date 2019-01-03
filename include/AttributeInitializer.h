#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include "AttributeInitializerBase.h"

namespace ORMPlusPlus{

/**
 * used at initialization of user model attributes, AttribType is one of NullableField<T> specializations<br>
 * ex: String, Integer, Long
 */
template<class AttribType>
class AttributeInitializer : public AttributeInitializerBase{
private:
	AttribType nullableField;
public:
	AttributeInitializer(AttributeInitializer& that) = delete;

	/**
	 * @param NFBase reference to field to be initialized
	 * @param associatedColumn pointer to associated column to be initialized, nullptr in case it is already setup
	 */
	AttributeInitializer(NullableFieldBase& NFBase, TableColumn* associatedColumn)
	: AttributeInitializerBase(NFBase, associatedColumn), nullableField(NFBase)
	{
	}

	template<class primitiveType>
	AttributeInitializer& withDefault(primitiveType value){
		//TODO: check operator= is implemented correctly
		nullableField = value;
		return *this;
	}

	//TODO: add more supported types for auto increment
	template < class T = AttribType,
	           class = typename std::enable_if<std::is_same<T, Integer>::value>::type>//only allow auto increment with integers -at least for now-
	AttributeInitializer& autoIncrement() {
		AttributeInitializerBase::autoIncrement();
		return *this;
	}

	~AttributeInitializer(){
	}

//	operator AttribType&&(){
//		//FIXME: which recipient ctor will be called here ?
//		return std::move(nullableField);
//	}
};

}
#endif /* INCLUDE_ATTRIBUTEINITIALIZER_H_ */
