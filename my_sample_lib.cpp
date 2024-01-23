#include "my_sample_lib.h"
#include <fmt/core.h>

void greet(const std::string& name) {
    fmt::print("Hello, {}!\n", name);
}
