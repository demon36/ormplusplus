#include <type_info.h>
#include <string>
#include <stdexcept>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

using namespace std;

namespace ormplusplus {

type_info::type_info(const std::type_info& wrapper_type, const std::type_info& primitive_type, bool _is_integral, bool _is_text)
: wrapper_type_hash(wrapper_type.hash_code()), primitive_type_hash(primitive_type.hash_code()), is_integral(_is_integral), is_text(_is_text), name(get_type_name(wrapper_type))
{}

type_info::type_info(const type_info& that)
: wrapper_type_hash(that.wrapper_type_hash), primitive_type_hash(that.primitive_type_hash), is_integral(that.is_integral), is_text(that.is_text), name(that.name)
{}

//TODO: convert to template fn
string type_info::get_type_name(const std::type_info& type){
#ifdef __GNUC__
	int status;
	char* demangled = abi::__cxa_demangle(type.name(),0,0,&status);
	if(status != 0){
		throw runtime_error("failed to demangle class name at model_base::get_type_name");
	}
	std::string demangled_str(demangled);
	free(demangled);
	return demangled_str;
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
