#include "TypeInfo.h"

#include <string>
#include <stdexcept>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

using namespace std;

namespace ORMPlusPlus {

TypeInfo::TypeInfo(const std::type_info& type, bool _isIntegral, bool _isText)
: nullableTypeHash(type.hash_code()), isIntegral(_isIntegral), isText(_isText), name(getTypeName(type))
{}

TypeInfo::TypeInfo(const TypeInfo& that)
: nullableTypeHash(that.nullableTypeHash), isIntegral(that.isIntegral), isText(that.isText), name(that.name)
{}

string TypeInfo::getTypeName(const std::type_info& type){
#ifdef __GNUC__
	int status;
	char* demangled = abi::__cxa_demangle(type.name(),0,0,&status);
	if(status != 0){
		throw runtime_error("failed to demangle class name at ModelBase::getTypeName");
	}
	std::string demangledStr(demangled);
	free(demangled);
	return demangledStr;
#else
	//msvc & clang do not -presumably- mangle class names
	//todo: test on both
	return type.name();
#endif
}

bool operator==(const TypeInfo lhs, const TypeInfo rhs){
	return lhs.nullableTypeHash == rhs.nullableTypeHash &&
		lhs.isIntegral == rhs.isIntegral &&
		lhs.isText == rhs.isText;
}

}
