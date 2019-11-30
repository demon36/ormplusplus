#ifndef INCLUDE_NULLABLEFIELDBASE_H_
#define INCLUDE_NULLABLEFIELDBASE_H_

#include <cstddef>
#include <string>
#include <sstream>
#include <typeindex>
#include <list>

namespace ORMPlusPlus {

//TODO: remove this after removing NullableField<nullptr_t>
//needed for using NullableField<nullptr_t>
std::ostream& operator<<(std::ostream& outstream, nullptr_t value);

struct TypeInfo
{
	TypeInfo() = delete;
	const size_t nullableTypeHash;
	const bool isIntegral;
	const bool isText;

	static const TypeInfo Int32Type;
	static const TypeInfo Int64Type;
	static const TypeInfo FloatType;
	static const TypeInfo DoubleType;
	static const TypeInfo StringType;
	static const TypeInfo DateTimeType;
	static const TypeInfo NullType;

	static const std::list<TypeInfo> AllTypes;
	static const TypeInfo& findbyHash(size_t hash);
};


bool operator==(const TypeInfo lhs, const TypeInfo rhs);

/**
 * Encapsulates the nullable field data
 */
class NullableFieldBase{
private:
	void* primitiveValuePtr = nullptr;
	const std::size_t typeHash;
	bool hasValue = false;

public:

	template<class PrimitiveType>
	static NullableFieldBase create(const PrimitiveType& value){
		NullableFieldBase instance(typeid(PrimitiveType).hash_code());
		instance.setValue<PrimitiveType>(value);
		return instance;
	}

	NullableFieldBase();
	NullableFieldBase(size_t typeHash);//todo: replace with a fancy TypeInfo class for distinction from long int or whatever
	NullableFieldBase(const NullableFieldBase& that);
	NullableFieldBase(NullableFieldBase&& that);
	NullableFieldBase& operator=(const NullableFieldBase& that);

	template<class PrimitiveType>
	NullableFieldBase& operator=(const PrimitiveType& value){
		//TODO: add fn assert type ?
		if(typeid(PrimitiveType).hash_code() != typeHash){//todo: fix assertion done twice
			throw std::runtime_error("trying to assign nullable field to non-compatible type value");
		}
		getValueRef<PrimitiveType>() = value;
		hasValue = true;
		return *this;
	}

	template<class PrimitiveType>
	PrimitiveType& getValueRef() const{
		//TODO: assert not null
		if(typeid(PrimitiveType).hash_code() == typeHash){
			return *(PrimitiveType*)primitiveValuePtr;
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::getValueRef()");
		}
	}

	template<class PrimitiveType>
	void setValue(const PrimitiveType& value){
		//TODO: assert not null
		//TODO: use enable if
		if(typeid(PrimitiveType).hash_code() == typeHash){
			*(PrimitiveType*)primitiveValuePtr = value;
			hasValue = true;//todo: replace hasValue with valuePtr != nullptr
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::setValue(value)");
		}
	}

	bool equals(const NullableFieldBase& that) const;
	bool isNull() const;
	std::string toString() const;
	size_t getType() const;

	//TODO: support bypassing to streams
	~NullableFieldBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_NULLABLEFIELDBASE_H_ */
