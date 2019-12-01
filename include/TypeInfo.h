#ifndef INCLUDE_TYPEINFO_H_
#define INCLUDE_TYPEINFO_H_

#include <list>
#include <string>

namespace ORMPlusPlus {

class TypeInfo
{
public:
	TypeInfo() = delete;
	TypeInfo(TypeInfo&&) = delete;
	TypeInfo(const std::type_info& wrapperType, const std::type_info& primitiveType, bool _isIntegral, bool _isText);
	TypeInfo(const TypeInfo& that);

	const size_t wrapperTypeHash; //like typeid(NullableField<long>)
	const size_t primitiveTypeHash; //like typeid(long)
	const bool isIntegral;
	const bool isText;
	const std::string name;//useful for debugging

	static std::string getTypeName(const std::type_info& type);
};

bool operator==(const TypeInfo lhs, const TypeInfo rhs);
bool operator!=(const TypeInfo lhs, const TypeInfo rhs);
}


#endif /* INCLUDE_TYPEINFO_H_ */
