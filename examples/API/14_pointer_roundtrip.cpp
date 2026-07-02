#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_pointer_roundtrip");
    auto* function = module.create_function<std::int64_t(void*)>("ptr_bits");
    auto* entry = function->create_block("entry");
    auto bits = entry->ptrtoint(jiterati::Type::i64(), entry->arg(0));
    auto ptr = entry->inttoptr(bits);
    entry->ret(entry->ptrtoint(jiterati::Type::i64(), ptr));
    std::cout << module.to_string();
    return 0;
}
