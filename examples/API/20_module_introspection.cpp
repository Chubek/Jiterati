#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_module_introspection");
    auto* function = module.create_function<int(int, int)>("accumulate");
    auto* entry = function->create_block("entry");
    entry->ret(entry->add(entry->arg(0), entry->arg(1)));

    std::cout << module.name() << '\n';
    std::cout << function->name() << ' ' << function->param_count() << '\n';
    std::cout << function->entry_block()->name() << '\n';
    std::cout << function->to_string();
    return function->const_count() == 0 ? 0 : 1;
}
