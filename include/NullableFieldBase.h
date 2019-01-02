#ifndef INCLUDE_NULLABLEFIELDBASE_H_
#define INCLUDE_NULLABLEFIELDBASE_H_

#include <string>
#include <sstream>
#include <iostream>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>

namespace ORMPlusPlus {

template<class PrimitiveType>
class NullableField;

typedef NullableField<int> Integer;
typedef NullableField<long> Long;
typedef NullableField<float> Float;
typedef NullableField<double> Double;
typedef NullableField<std::string> String;
typedef NullableField<Poco::DateTime> DateTime;
typedef NullableField<nullptr_t> Null;

//needed for using NullableField<nullptr_t>
std::ostream& operator<<(std::ostream& outstream, nullptr_t value);

class NullableFieldBase{

private:
	void* primitiveValuePtr = nullptr;
	const std::type_info& m_type;
	bool hasValue = false;

protected:
	static void assertLHSNotNull(const NullableFieldBase& lhs);
	static void assertRHSNotNull(const NullableFieldBase& rhs);

public:
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

	template<class PrimitiveType>
	const PrimitiveType& getValueRef() const{
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
	std::string toString();

	~NullableFieldBase();
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_NULLABLEFIELDBASE_H_ */
