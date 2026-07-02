#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_bitwise_mask");
    auto* function = module.create_function<int(int)>("mask_low_byte");
    auto* entry = function->create_block("entry");
    auto mask = function->const_i32(255);
    entry->ret(entry->bitwise_and(entry->arg(0), mask));
    std::cout << module.to_string();
    return 0;
}
