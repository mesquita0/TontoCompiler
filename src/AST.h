#pragma once
#include <list>
#include <string>

class Node {      
    std::string name;
    std::list<Node*> children;

public:
    Node(std::string name) : name(name) { }

    void add_child(Node* node) {
        children.push_back(node);
    }
};

class Package : public Node {
public:
    Package(std::string name) : Node(name) { }
};

class Class : public Node {
public:
    Class(std::string name) : Node(name) { }
};

class Attribute : public Node {
    std::string data_type;

public:
    Attribute(std::string name, std::string data_type) : Node(name), data_type(data_type) { }
};

class Relation : public Node {
    bool is_internal;
    std::string related_class;

public:
    Relation(std::string related_class) : Node(""), is_internal(true), related_class(related_class) { }

    Relation(std::string name, std::string related_class) : Node(name), is_internal(false), related_class(related_class) { }
};

class Type : public Node {
public:
    Type(std::string name) : Node(name) { }
};

class Enum : public Node {
public:
    Enum(std::string name) : Node(name) { }
};

class Instance : public Node {
public:
    Instance(std::string name) : Node(name) { }
};

class Genset : public Node {
    std::string mother_class;

public:
    Genset(std::string name, std::string mother_class) : Node(name), mother_class(mother_class) { }

    void setMotherClass(std::string mother_class) { this->mother_class = mother_class; }
};

class AST {
    Node root = Node("root");

public:
    Node* add_package(std::string name);
    Node* add_class(Package* package, std::string name);
    Node* add_attribute(Node* node, std::string name, std::string type);
    Node* add_relation(Class* src_class, std::string related_class); // internal relation
    Node* add_relation(Package* package, std::string name, std::string related_class); // external relation
    Node* add_type(Package* package, std::string name);
    Node* add_enum(Package* package, std::string name);
    Node* add_instance(Enum* enumeration, std::string name);
    Node* add_genset(Package* package, std::string name);
};
