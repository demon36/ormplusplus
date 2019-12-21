#ifndef INCLUDE_TYPE_INFO_H_
#define INCLUDE_TYPE_INFO_H_

#include <list>
#include <string>

namespace ormplusplus {

class type_info
{
public:
	type_info() = delete;
	type_info(type_info&&) = delete;
	type_info(const std::type_info& wrapper_type, const std::type_info& primitive_type, bool _is_integral, bool _is_text);
	type_info(const type_info& that);

	const size_t wrapper_type_hash; //like typeid(NullableField<long>)
	const size_t primitive_type_hash; //like typeid(long)
	const bool is_integral;
	const bool is_text;
	const std::string name;//useful for debugging

	static std::string get_type_name(const std::type_info& type);
};

bool operator==(const type_info lhs, const type_info rhs);
bool operator!=(const type_info lhs, const type_info rhs);
}


#endif /* INCLUDE_TYPE_INFO_H_ */
