#ifndef INCLUDE_NULLABLEFIELDBASE_H_
#define INCLUDE_NULLABLEFIELDBASE_H_

#include "TypeInfo.h"

#include <cstddef>
#include <string>
#include <sstream>
#include <typeindex>

namespace ORMPlusPlus {

//TODO: remove this after removing NullableField<nullptr_t>
//needed for using NullableField<nullptr_t>
std::ostream& operator<<(std::ostream& outstream, nullptr_t value);

/**
 * Encapsulates the nullable field data
 */
class NullableFieldBase{
private:
	void* primitiveValuePtr = nullptr;
	const TypeInfo& typeInfoRef;
	bool hasValue = false;

public:

	template<class PrimitiveType>
	static NullableFieldBase create(const TypeInfo& typeInfo, const PrimitiveType& value){
		NullableFieldBase instance(typeInfo);
		instance.setValue<PrimitiveType>(value);
		return instance;
	}

	NullableFieldBase();
	NullableFieldBase(const TypeInfo& typeInfo);
	NullableFieldBase(const NullableFieldBase& that);
	NullableFieldBase(NullableFieldBase&& that);
	NullableFieldBase& operator=(const NullableFieldBase& that);

	template<class PrimitiveType>
	NullableFieldBase& operator=(const PrimitiveType& value){
		//TODO: add fn assert type ?
		if(typeid(PrimitiveType).hash_code() != typeInfoRef.primitiveTypeHash){//todo: fix assertion done twice
			throw std::runtime_error("trying to assign nullable field to non-compatible type value");
		}
		getValueRef<PrimitiveType>() = value;
		hasValue = true;
		return *this;
	}

	template<class PrimitiveType>
	PrimitiveType& getValueRef() const{
		//TODO: assert not null
		if(typeid(PrimitiveType).hash_code() == typeInfoRef.primitiveTypeHash){
			return *(PrimitiveType*)primitiveValuePtr;
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::getValueRef()");
		}
	}

	template<class PrimitiveType>
	void setValue(const PrimitiveType& value){
		//TODO: assert not null
		//TODO: use enable if
		if(typeid(PrimitiveType).hash_code() == typeInfoRef.primitiveTypeHash){
			*(PrimitiveType*)primitiveValuePtr = value;
			hasValue = true;//todo: replace hasValue with valuePtr != nullptr
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::setValue(value)");
		}
	}

	bool equals(const NullableFieldBase& that) const;
	bool isNull() const;
	std::string toString() const;
	const TypeInfo& getType() const;
	static bool isIntegral(const std::type_info& type);
	static bool isText(const std::type_info& type);

	//TODO: support bypassing to streams
	~NullableFieldBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_NULLABLEFIELDBASE_H_ */
