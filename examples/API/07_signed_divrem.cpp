#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_signed_divrem");
    auto* function = module.create_function<int(int, int)>("divrem_score");
    auto* entry = function->create_block("entry");
    auto quotient = entry->sdiv(entry->arg(0), entry->arg(1));
    auto remainder = entry->srem(entry->arg(0), entry->arg(1));
    entry->ret(entry->add(entry->mul(quotient, function->const_i32(10)), remainder));
    std::cout << module.to_string();
    return 0;
}
