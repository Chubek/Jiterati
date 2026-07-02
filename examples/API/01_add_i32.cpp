#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_add_i32");
    auto* function = module.create_function<int(int, int)>("add_i32");
    auto* entry = function->create_block("entry");
    entry->ret(entry->add(entry->arg(0), entry->arg(1)));
    std::cout << module.to_string();
    return 0;
}
