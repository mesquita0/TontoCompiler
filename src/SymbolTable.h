#pragma once
#include <unordered_map>
#include <string>

class Node;

class SymbolTable {
    std::unordered_map<std::string, Node> symbols;

public:
    Node* add(const Node& node);
    Node* get(const std::string& name);
};
