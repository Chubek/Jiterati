#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_unsigned_divrem");
    auto* function = module.create_function<std::uint32_t(std::uint32_t, std::uint32_t)>("udivrem_score");
    auto* entry = function->create_block("entry");
    auto quotient = entry->udiv(entry->arg(0), entry->arg(1));
    auto remainder = entry->urem(entry->arg(0), entry->arg(1));
    entry->ret(entry->bitwise_or(entry->shl(quotient, function->const_i32(16)), remainder));
    std::cout << module.to_string();
    return 0;
}
