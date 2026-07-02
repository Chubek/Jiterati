#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_void_return");
    auto* function = module.create_function<void()>("do_nothing");
    auto* entry = function->create_block("entry");
    entry->ret_void();
    std::cout << module.to_string();
    return 0;
}
