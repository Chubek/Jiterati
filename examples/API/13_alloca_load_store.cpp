#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_alloca_load_store");
    auto* function = module.create_function<int(int)>("spill_reload");
    auto* entry = function->create_block("entry");
    auto slot = entry->alloca(jiterati::Type::i32());
    entry->store(slot, entry->arg(0));
    auto loaded = entry->load(jiterati::Type::i32(), slot);
    entry->ret(entry->add(loaded, function->const_i32(4)));
    std::cout << module.to_string();
    return 0;
}
