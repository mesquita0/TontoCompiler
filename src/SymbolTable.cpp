#include "SymbolTable.h"
#include "AST.h"
#include <unordered_map>
#include <string>

Node* SymbolTable::add(Node* node) {
    if (node->getName() == "")
        node->setName("unnamed" + i++);

    if (symbols.contains(node->getName())) 
        delete node;
    else
        symbols.emplace(node->getName(), node);

    return get(node->getName());
}

Node* SymbolTable::get(const std::string& name) {
    return symbols.at(name);
}

SymbolTable::~SymbolTable() {
    for (auto& [name, node] : symbols)
        delete node;
}
