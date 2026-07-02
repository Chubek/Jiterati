#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_min_select");
    auto* function = module.create_function<int(int, int)>("min_i32");
    auto* entry = function->create_block("entry");
    auto lhs = entry->arg(0);
    auto rhs = entry->arg(1);
    auto take_lhs = entry->icmp_sle(lhs, rhs);
    entry->ret(entry->select(take_lhs, lhs, rhs));
    std::cout << module.to_string();
    return 0;
}
