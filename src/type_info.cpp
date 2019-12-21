#include <type_info.h>
#include <string>
#include <stdexcept>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

using namespace std;

namespace ormplusplus {

type_info::type_info(const std::type_info& wrapperType, const std::type_info& primitiveType, bool _isIntegral, bool _isText)
: wrapper_type_hash(wrapperType.hash_code()), primitive_type_hash(primitiveType.hash_code()), is_integral(_isIntegral), is_text(_isText), name(get_type_name(wrapperType))
{}

type_info::type_info(const type_info& that)
: wrapper_type_hash(that.wrapper_type_hash), primitive_type_hash(that.primitive_type_hash), is_integral(that.is_integral), is_text(that.is_text), name(that.name)
{}

string type_info::get_type_name(const std::type_info& type){
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

bool operator==(const type_info lhs, const type_info rhs){
	return lhs.wrapper_type_hash == rhs.wrapper_type_hash;
}

bool operator!=(const type_info lhs, const type_info rhs){
	return !operator ==(lhs, rhs);
}

}
