#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_conditional_branch");
    auto* function = module.create_function<int(int)>("signum_branch");
    auto* entry = function->create_block("entry");
    auto* negative = function->create_block("negative");
    auto* non_negative = function->create_block("non_negative");
    auto zero = function->const_i32(0);
    entry->cond_br(entry->icmp_slt(entry->arg(0), zero), negative, non_negative);
    negative->ret(function->const_i32(-1));
    non_negative->ret(function->const_i32(1));
    std::cout << module.to_string();
    return 0;
}
