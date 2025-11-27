#include "AST.h"
#include <list>
#include <string>

Node* AST::add_package(std::string name) {
    Node* package = new Package(name);
    root.add_child(package);

    return package;
}

Node* AST::add_class(Package* package, std::string name) {
    Node* declared_class = new Class(name);
    package->add_child(declared_class);

    return declared_class;
}

Node* AST::add_attribute(Node* node, std::string name, std::string type) {
    Node* attribute = new Attribute(name, type);
    node->add_child(attribute);

    return attribute;
}

Node* AST::add_relation(Class* src_class, std::string related_class) {
    Node* relation = new Relation(related_class);
    src_class->add_child(relation);

    return relation;
}

Node* AST::add_relation(Package* package, std::string name, std::string related_class) {
    Node* relation = new Relation(name, related_class);
    package->add_child(relation);

    return relation;
}

Node* AST::add_type(Package* package, std::string name) {
    Node* type = new Type(name);
    package->add_child(type);

    return type;
}

Node* AST::add_enum(Package* package, std::string name) {
    Node* enumeration = new Enum(name);
    package->add_child(enumeration);

    return enumeration;
}

Node* AST::add_instance(Enum* enumeration, std::string name) {
    Node* instance = new Instance(name);
    enumeration->add_child(instance);

    return instance;
}

Node* AST::add_genset(Package* package, std::string name) {
    Node* genset = new Genset(name, "");
    package->add_child(genset);

    return genset;
}
