#include "SymbolTable.h"
#include "AST.h"
#include <unordered_map>
#include <string>
#include <typeinfo>

Node* SymbolTable::add(Node* node) {
    if (node->getName() == "")
        node->setName("unnamed" + i++);

    std::string name = node->getName();

    if (!symbols.contains(name))
        symbols.emplace(name, node);
    else {
        if (typeid(*node) != typeid(*get(node->getName()))) return nullptr;
        delete node;
    }

    return get(name);
}

Node* SymbolTable::get(const std::string& name) {
    return symbols.at(name);
}

SymbolTable::~SymbolTable() {
    for (auto& [name, node] : symbols)
        delete node;
}
