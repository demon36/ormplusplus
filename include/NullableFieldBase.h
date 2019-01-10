#ifndef INCLUDE_NULLABLEFIELDBASE_H_
#define INCLUDE_NULLABLEFIELDBASE_H_

#include <string>
#include <sstream>
#include <iostream>
#include <typeindex>
#include <map>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>

namespace ORMPlusPlus {

//TODO: remove this after removing NullableField<nullptr_t>
//needed for using NullableField<nullptr_t>
std::ostream& operator<<(std::ostream& outstream, nullptr_t value);

//some type information
struct TypeInfo
{
	const bool isIntegral;
	const bool isText;
	const std::string DBName;
};

/**
 * Encapsulates the nullable field data
 */
class NullableFieldBase{

private:
	void* primitiveValuePtr = nullptr;
	const std::type_info& m_type;
	bool hasValue = false;

protected:
	static void assertLHSNotNull(const NullableFieldBase& lhs);
	static void assertRHSNotNull(const NullableFieldBase& rhs);

public:

	//key: type_info hash
	//TODO: add getter for the map
	static const std::map<std::size_t, TypeInfo> typeInfoMap;
	static std::size_t getTypeHash(std::string TypeDBName);

	template<class PrimitiveType>
	static NullableFieldBase create(const PrimitiveType& value){
		NullableFieldBase instance(typeid(PrimitiveType));
		instance.setValue<PrimitiveType>(value);
		return instance;
	}

	NullableFieldBase();
	NullableFieldBase(const std::type_info& type);
	NullableFieldBase(const NullableFieldBase& that);
	NullableFieldBase(NullableFieldBase&& that);
	NullableFieldBase& operator=(const NullableFieldBase);

	template<class PrimitiveType>
	NullableFieldBase& operator=(const PrimitiveType& value){
		//TODO: add fn assert type ?
		if(typeid(PrimitiveType) != m_type){//FIXME: assertion done twice
			throw std::runtime_error("trying to assign nullable field to non-compatible type value");
		}
		getValueRef<PrimitiveType>() = value;
		hasValue = true;
	}

	template<class PrimitiveType>
	PrimitiveType& getValueRef() const{
		//TODO: assert not null
		if(typeid(PrimitiveType) == m_type){
			return *(PrimitiveType*)primitiveValuePtr;
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::getValueRef()");
		}
	}

	template<class PrimitiveType>
	void setValue(const PrimitiveType& value){
		//TODO: assert not null
		if(typeid(PrimitiveType) == m_type){
			*(PrimitiveType*)primitiveValuePtr = value;
			hasValue = true;
		}else{
			throw std::runtime_error("type mismatch at NullableFieldBase::getValueRef()");
		}
	}

	bool isNull() const;
	std::string toString() const;
	const std::type_info& getType() const;

	//TODO: support bypassing to streams
	~NullableFieldBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_NULLABLEFIELDBASE_H_ */
