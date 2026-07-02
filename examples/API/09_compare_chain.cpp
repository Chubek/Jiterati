#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_compare_chain");
    auto* function = module.create_function<bool(int, int)>("ordered_pair");
    auto* entry = function->create_block("entry");
    auto less = entry->icmp_slt(entry->arg(0), entry->arg(1));
    auto not_equal = entry->icmp_ne(entry->arg(0), entry->arg(1));
    entry->ret(entry->bitwise_and(less, not_equal));
    std::cout << module.to_string();
    return 0;
}
