#include "Jiterati.hpp"

#include <iostream>

int main() {
    jiterati::Module module("api_phi_loop");
    auto* function = module.create_function<int(int)>("countdown_seed");
    auto* entry = function->create_block("entry");
    auto* loop = function->create_block("loop");
    auto* exit = function->create_block("exit");
    entry->br(loop);
    auto induction = loop->phi(jiterati::Type::i32(), {{entry, function->const_i32(0)}, {loop, entry->arg(0)}});
    auto next = loop->add(induction, function->const_i32(1));
    loop->cond_br(loop->icmp_slt(next, entry->arg(0)), loop, exit);
    exit->ret(next);
    std::cout << module.to_string();
    return 0;
}
