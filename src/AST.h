#pragma once
#include "SymbolTable.h"
#include <list>
#include <string>

class Node {
    std::string name;
    std::list<Node*> children;

public:
    Node(std::string name) : name(name) { }

    virtual ~Node() = default;

    void setName(const std::string& name) { this->name = name; }
    void add_child(Node* node) { children.push_back(node); }
    const std::string& getName() const { return name; }
    std::list<Node*>& getChildren() { return children; }
};

class Package : public Node {
public:
    Package(std::string name) : Node(name) { }
};

class Class : public Node {
    std::string stereotype;
    std::list<std::string> super_classes;

public:
    Class(std::string stereotype, std::string name) : Node(name), stereotype(stereotype) { }

    void addSuperClass(const std::string& sc) { super_classes.push_back(sc); }

    const std::string& getStereotype() const { return stereotype; }
    const std::list<std::string>& getSuperClasses() const { return super_classes; }
};

class Attribute : public Node {
    std::string data_type;

public:
    Attribute(std::string name, std::string data_type) : Node(name), data_type(data_type) { }

    const std::string& getDataType() const { return data_type; }
};

class Relation : public Node {
    bool is_internal;
    std::string stereotype;
    std::string domain_class, image_class;

public:
    Relation(std::string stereotype, std::string image_class) 
        : Node(""), is_internal(true), stereotype(stereotype), image_class(image_class) 
    { }

    Relation(std::string stereotype, std::string domain_class, std::string image_class)
        : Node(""), is_internal(false), stereotype(stereotype), domain_class(domain_class), image_class(image_class) 
    { }

    const std::string& getStereotype() const { return stereotype; }
    const std::string& getDomain() const { return domain_class; }
    const std::string& getImage()  const { return image_class; }

    bool isInternal() const { return is_internal; }  
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
    std::list<std::string> classes;
    bool disjoint = false, complete = false;

public:
    Genset(std::string name, std::string mother_class) : Node(name), mother_class(mother_class) { }

    void setMotherClass(std::string mother_class) { this->mother_class = mother_class; }
    void addClass(std::string gen_class) { this->classes.push_back(gen_class); }

    const std::string& getMotherClass() const { return mother_class; }
    const std::list<std::string>& getSpecificClass() const { return classes; }

    void setDisjoint() { disjoint = true; }
    void setComplete() { complete = true; }

    bool isDisjoint() const { return disjoint; }
    bool isComplete() const { return complete; }
};

class AST {
    Node root = Node("root");
    SymbolTable& symbol_table;

    void link_relations();

public:
    AST(SymbolTable& symbol_table) : symbol_table(symbol_table) { }

    Node* add_package(std::string name);
    Node* add_class(Package* package, std::string stereotype, std::string name);
    Node* add_attribute(Node* node, std::string name, std::string type);
    Node* add_relation(Class* src_class, std::string stereotype, std::string related_class); // internal relation
    Node* add_relation(Package* package, std::string stereotype, std::string name, std::string related_class); // external relation
    Node* add_type(Package* package, std::string name);
    Node* add_enum(Package* package, std::string name);
    Node* add_instance(Enum* enumeration, std::string name);
    Node* add_genset(Package* package, std::string name);

    void consolidate();
    void print_summary();
};
