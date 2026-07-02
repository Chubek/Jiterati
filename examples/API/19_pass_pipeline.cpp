#include "Jiterati-Pass.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_pass_pipeline");
    auto* function = module.create_function<int(int)>("fold_candidate");
    auto* entry = function->create_block("entry");
    auto folded = entry->mul(function->const_i32(6), function->const_i32(7));
    entry->ret(entry->add(entry->arg(0), folded));

    auto pipeline = jiterati::parse_jpl_pipeline("cfg; constant-folding; dce");
    pipeline.run(module);
    std::cout << module.to_string();
    return 0;
}
