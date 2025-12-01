#include "AST.h"
#include "SymbolTable.h"
#include <iostream>
#include <list>
#include <string>

Node* AST::add_package(std::string name) {
    Node package = Package(name);
    Node* new_node = symbol_table.add(package);
    root.add_child(new_node);

    return new_node;
}

Node* AST::add_class(Package* package, std::string name) {
    Node declared_class = Class(name);
    Node* new_node = symbol_table.add(declared_class);
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_attribute(Node* node, std::string name, std::string type) {
    Node attribute = Attribute(name, type);
    Node* new_node = symbol_table.add(attribute);
    node->add_child(new_node);

    return new_node;
}

Node* AST::add_relation(Class* src_class, std::string related_class) {
    Node relation = Relation(related_class);
    Node* new_node = symbol_table.add(relation);
    src_class->add_child(new_node);

    return new_node;
}

Node* AST::add_relation(Package* package, std::string domain, std::string image) {
    Node relation = Relation(domain, image);
    Node* new_node = symbol_table.add(relation);
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_type(Package* package, std::string name) {
    Node type = Type(name);
    Node* new_node = symbol_table.add(type);
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_enum(Package* package, std::string name) {
    Node enumeration = Enum(name);
    Node* new_node = symbol_table.add(enumeration);
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_instance(Enum* enumeration, std::string name) {
    Node instance = Instance(name);
    Node* new_node = symbol_table.add(instance);
    enumeration->add_child(new_node);

    return new_node;
}

Node* AST::add_genset(Package* package, std::string name) {
    Node genset = Genset(name, "");
    Node* new_node = symbol_table.add(genset);
    package->add_child(new_node);

    return new_node;
}

void AST::link_relations() {
    for (Node* package : root.getChildren()) {
        std::list<Node*>& nodes = package->getChildren();
        auto it = nodes.begin();

        while (it != nodes.end()) {
            Relation* external_relation = dynamic_cast<Relation*>(*it);
            if (!external_relation) {
                it++;
                continue;
            }

            Node* domain_class = symbol_table.get(external_relation->getDomain());
            domain_class->add_child(external_relation);

            it = nodes.erase(it);
        }
    }
}

