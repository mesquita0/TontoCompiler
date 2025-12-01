#include "AST.h"
#include "SymbolTable.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>

Node* AST::add_package(std::string name) {
    Node* new_node = symbol_table.add(new Package(name));
    root.add_child(new_node);

    return new_node;
}

Node* AST::add_class(Package* package, std::string name) {
    Node* new_node = symbol_table.add(new Class(name));
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_attribute(Node* node, std::string name, std::string type) {
    Node* new_node = symbol_table.add(new Attribute(name, type));
    node->add_child(new_node);

    return new_node;
}

Node* AST::add_relation(Class* src_class, std::string related_class) {
    Node* new_node = symbol_table.add(new Relation(related_class));
    src_class->add_child(new_node);

    return new_node;
}

Node* AST::add_relation(Package* package, std::string domain, std::string image) {
    Node* new_node = symbol_table.add(new Relation(domain, image));
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_type(Package* package, std::string name) {
    Node* new_node = symbol_table.add(new Type(name));
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_enum(Package* package, std::string name) {
    Node* new_node = symbol_table.add(new Enum(name));
    package->add_child(new_node);

    return new_node;
}

Node* AST::add_instance(Enum* enumeration, std::string name) {
    Node* new_node = symbol_table.add(new Instance(name));
    enumeration->add_child(new_node);

    return new_node;
}

Node* AST::add_genset(Package* package, std::string name) {
    Node* new_node = symbol_table.add(new Genset(name, ""));
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

void AST::print_summary() {
    link_relations();

    std::cout << "Synthesis Table\n\n";

    for (Node* package : root.getChildren()) {
        Package* pkg = dynamic_cast<Package*>(package);
        if (!pkg) continue;

        std::vector<Class*>  classes;
        std::vector<Type*>   types;
        std::vector<Enum*>   enums;
        std::vector<Genset*> gensets;

        std::cout << "Package: " << package->getName() << "\n";

        for (Node* child : package->getChildren()) {
            if      (auto cls    = dynamic_cast<Class*>(child))  classes.push_back(cls);
            else if (auto type   = dynamic_cast<Type*>(child))   types.push_back(type);
            else if (auto enm    = dynamic_cast<Enum*>(child))   enums.push_back(enm);
            else if (auto genset = dynamic_cast<Genset*>(child)) gensets.push_back(genset);
        }

        std::cout << "Number of Classes: " << (classes.size()) << "\n"
                  << "Number of Types:   " << (types.size()) << "\n"
                  << "Number of Enums:   " << (enums.size()) << "\n"
                  << "Number of Gensets: " << (gensets.size()) << "\n"
                  << "\n";

        if (!classes.empty())  {
            std::cout << "Classes\n";

            for (Class* cls : classes) {
                std::cout << "Class: " << cls->getName() << '\n';

                std::vector<std::string> attributes, relations;

                for (Node* elem : cls->getChildren())
                {
                    if (auto a = dynamic_cast<Attribute*>(elem))
                        attributes.push_back(a->getName() + ": " + a->getDataType());
                    else if (auto r = dynamic_cast<Relation*>(elem))
                        relations.push_back(r->getImage() + (r->isInternal() ? " (internal)" : " (external)"));
                }

                if (!attributes.empty()) {
                    std::cout << "Attributes: ";
                    for (auto& att : attributes) std::cout << att << '\n';
                }

                if (!relations.empty()) {
                    std::cout << "Relations:\n";
                    for (auto& rel : relations) std::cout << rel << '\n';
                }

                if (!attributes.empty() || !relations.empty())
                    std::cout << "\n";
            }

            std::cout << "\n";
        }

        if (!types.empty()) {
            std::cout << "Datatypes\n";

            for (Type* type : types) {
                std::cout << "Datatype: " << type->getName() << '\n';

                std::vector<std::string> attributes;

                for (Node* elem : type->getChildren())
                    if (auto a = dynamic_cast<Attribute*>(elem))
                        attributes.push_back(a->getName() + ": " + a->getDataType());

                if (!attributes.empty()) {
                    std::cout << "Attributes: ";
                    for (auto& att : attributes) std::cout << att << '\n';
                    std::cout << "\n";
                }
            }

            std::cout << "\n";
        }

        if (!enums.empty()) {
            std::cout << "Enums\n";

            for (Enum* enm : enums) {
                std::cout << "Enum: " << enm->getName() << '\n';

                std::vector<std::string> instances;

                for (Node* elem : enm->getChildren())
                    if (auto i = dynamic_cast<Instance*>(elem))
                        instances.push_back(i->getName());

                if (!instances.empty()) {
                    std::cout << "Instances: ";
                    for (auto& inst : instances) std::cout << inst << '\n';
                    std::cout << "\n";
                }
            }

            std::cout << "\n";
        }

        if (!gensets.empty()) 
            std::cout << "Gensets\n";

        for (Genset* genset : gensets) {
            std::cout << "Genset: " << genset->getName() << '\n';

            std::cout << "Mother class: " << genset->getMotherClass() << '\n';

            if (!genset->getSpecificClass().empty()) {
                std::cout << "Specific classes:\n";
                for (auto& cls : genset->getSpecificClass()) std::cout << "- " << cls << '\n';
                std::cout << "\n";
            }
        }
    }
}
