#include "util.h"

#include <sstream>
#include <iomanip>

using namespace std;

namespace util{
    
void from_string(const std::string& src, int& dest){
    dest = stoi(src);
}

void from_string(const std::string& src, long& dest){
    dest = stol(src);
}

void from_string(const std::string& src, float& dest){
    dest = stof(src);
}

void from_string(const std::string& src, double& dest){
    dest = stod(src);
}
void from_string(const std::string& src, std::string& dest) {
    dest = src;
}

void from_string(const std::string& src, tm& dest){
    std::istringstream ss(src);
    ss >> std::get_time(&dest, "%Y-%m-%d %H:%M:%S");
}

}