#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_direct_call");
    auto* square = module.create_function<int(int)>("square");
    auto* square_entry = square->create_block("entry");
    square_entry->ret(square_entry->mul(square_entry->arg(0), square_entry->arg(0)));

    auto* caller = module.create_function<int(int)>("square_plus_one");
    auto* caller_entry = caller->create_block("entry");
    auto value = caller_entry->call(square, {caller_entry->arg(0)});
    caller_entry->ret(caller_entry->add(value, caller->const_i32(1)));
    std::cout << module.to_string();
    return 0;
}
