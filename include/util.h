#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util{

void from_string(const std::string& src, int& dest);
void from_string(const std::string& src, long& dest);
void from_string(const std::string& src, float& dest);
void from_string(const std::string& src, double& dest);
void from_string(const std::string& src, std::string& dest);
void from_string(const std::string& src, tm& dest);

}

#endif /* UTIL_H */

