#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_const_bool_select");
    auto* function = module.create_function<int()>("choose_constant");
    auto* entry = function->create_block("entry");
    entry->ret(entry->select(function->const_bool(true), function->const_i32(7), function->const_i32(9)));
    std::cout << module.to_string();
    return 0;
}
