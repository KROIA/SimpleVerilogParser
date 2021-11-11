#ifndef CORE_H
#define CORE_H

#include <regex>
#include <string>
#include <vector>
#include "debug.h"



#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

using std::string;
using std::to_string;
using std::vector;
using std::regex;
using std::smatch;

namespace Verilog {
    class Reg;

}


std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with);
std::size_t remove_all(std::string& inout, std::string_view what);

#endif // CORE_H
