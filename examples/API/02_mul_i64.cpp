#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_mul_i64");
    auto* function = module.create_function<std::int64_t(std::int64_t, std::int64_t)>("mul_i64");
    auto* entry = function->create_block("entry");
    entry->ret(entry->mul(entry->arg(0), entry->arg(1)));
    std::cout << module.to_string();
    return 0;
}
