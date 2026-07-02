#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_jbl_roundtrip");
    auto* function = module.create_function<int(int)>("identity");
    auto* entry = function->create_block("entry");
    entry->ret(entry->arg(0));

    std::string error;
    auto parsed = jiterati::parse_jbl(jiterati::print_jbl(module), &error);
    if (!parsed) {
        std::cerr << error << '\n';
        return 1;
    }
    std::cout << jiterati::print_jbl(*parsed);
    return 0;
}
