#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_shift_mix");
    auto* function = module.create_function<int(int, int)>("shift_mix");
    auto* entry = function->create_block("entry");
    auto x = entry->arg(0);
    auto amount = entry->arg(1);
    auto left = entry->shl(x, amount);
    auto right = entry->ashr(x, function->const_i32(3));
    entry->ret(entry->bitwise_xor(left, right));
    std::cout << module.to_string();
    return 0;
}
