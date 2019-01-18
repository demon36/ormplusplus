#ifndef INCLUDE_ATTRIBUTEINITIALIZER_H_
#define INCLUDE_ATTRIBUTEINITIALIZER_H_

#include "AttributeInitializerBase.h"
#include "NullableField.h"

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

	//TODO: since this is the only thing that implied making this class a template class
	//		we can just make 2 variants of it, a safe variant with template argument
	//		and another unsafe variant, this will -probably- eliminate the need for
	//		making this class a template class
	//TODO: get rid of the need to supply a template parameter to this function
	//		since the attribute type is already provided
	template<class primitiveType>
	AttributeInitializer& withDefault(primitiveType value){
		//TODO: check operator= is implemented correctly
		nullableField = value;
		AttributeInitializerBase::withDefault(nullableField.toString());
		return *this;
	}

	//TODO: add more supported types for auto increment
	template < class T = AttribType,
	           class = typename std::enable_if<std::is_same<T, Integer>::value>::type>//only allow auto increment with integers -at least for now-
	AttributeInitializer& autoIncrement() {
		AttributeInitializerBase::autoIncrement();
		return *this;
	}

	//TODO: throw exception if a decorator got called twice
	AttributeInitializer& asPrimary(){
		AttributeInitializerBase::asPrimary();
		return *this;
	}

	AttributeInitializer& asNullable(bool value = true){
		AttributeInitializerBase::asNullable();
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
