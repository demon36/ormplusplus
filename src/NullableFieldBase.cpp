#include "NullableFieldBase.h"

namespace ORMPlusPlus {

std::ostream& operator<<(std::ostream& outstream, nullptr_t value){
	return outstream;
}

} /* namespace ORMPlusPlus */
