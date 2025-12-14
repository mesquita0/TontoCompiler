#pragma once
#include <unordered_map>
#include <string>

class Node;

class SymbolTable {
    std::unordered_map<std::string, Node*> symbols;
    int i = 0;

public:
    ~SymbolTable();
    
    Node* add(Node* node);
    Node* get(const std::string& name);

    auto begin() { return symbols.begin(); }
    auto end()   { return symbols.end(); }

    auto begin() const { return symbols.begin(); }
    auto end()   const { return symbols.end(); }
};
