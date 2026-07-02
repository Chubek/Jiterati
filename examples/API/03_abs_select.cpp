#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_abs_select");
    auto* function = module.create_function<int(int)>("abs_i32");
    auto* entry = function->create_block("entry");
    auto x = entry->arg(0);
    auto zero = function->const_i32(0);
    auto negated = entry->sub(zero, x);
    auto negative = entry->icmp_slt(x, zero);
    entry->ret(entry->select(negative, negated, x));
    std::cout << module.to_string();
    return 0;
}
