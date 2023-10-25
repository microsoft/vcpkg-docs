#pragma once

#include <string>

#if MYLIB_EXPORTS
__declspec(dllexport)
#endif
std::string greet(const std::string& name);
