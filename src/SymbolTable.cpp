#include "SymbolTable.h"
#include "AST.h"
#include <unordered_map>
#include <string>

Node* SymbolTable::add(const Node& node) {
    symbols.try_emplace(node.getName(), node);

    return get(node.getName());
}

Node* SymbolTable::get(const std::string& name) {
    return &symbols.at(name);
}
