#include "SemanticAnalyzer.h"
#include "AST.h"
#include "SymbolTable.h"
#include <iostream>
#include <utility>
#include <string>

enum Pattern { Subkind, Role, Phase, Relator, Mode, RoleMixin };

std::pair<bool, bool> checkGenset(Genset* genset, const std::string& stereotype, SymbolTable& st);
void printResult(Pattern pattern, Node* node);

void SemanticAnalyzer::analyze() {
    ast.consolidate();

    for (auto& [name, node] : symbol_table) {
        if (auto* genset = dynamic_cast<Genset*>(node)) {
            checkSubkind(genset);
            checkRole(genset);
            checkPhase(genset);
            checkRoleMixin(genset);
        }
        else if (auto* cls = dynamic_cast<Class*>(node)) {
            checkRelator(cls);
            checkMode(cls);
        }
    }
}

void SemanticAnalyzer::checkSubkind(Genset* genset) {
    auto [all_subkinds, valid] = checkGenset(genset, "subkind", symbol_table);

    if (!all_subkinds) return;

    if (valid && !genset->isDisjoint()) {
        std::cout << "Genset \"" << genset->getName() << "\" must be disjoint in subkind pattern.\n";
        return;
    }

    if (valid) printResult(Subkind, genset);
}

void SemanticAnalyzer::checkRole(Genset* genset) {
    std::string mother_class = genset->getMotherClass();
    if (((Class*) symbol_table.get(mother_class))->getStereotype() == "roleMixin") return;

    auto [all_roles, valid] = checkGenset(genset, "role", symbol_table);

    if (!all_roles) return;

    if (valid && genset->isDisjoint()) {
        std::cout << "Genset \"" << genset->getName() << "\" cannot be disjoint in roles pattern.\n";
        return;
    }

    if (valid) printResult(Role, genset);
}

void SemanticAnalyzer::checkPhase(Genset* genset) {
    auto [all_phases, valid] = checkGenset(genset, "phase", symbol_table);

    if (!all_phases) return;

    if (valid && !genset->isDisjoint()) {
        std::cout << "Genset \"" << genset->getName() << "\" must be disjoint in phase pattern.\n";
        return;
    }

    if (valid) printResult(Phase, genset);
}

void SemanticAnalyzer::checkRoleMixin(Genset* genset) {
    std::string mother_class = genset->getMotherClass();

    if (((Class*) symbol_table.get(mother_class))->getStereotype() != "roleMixin") return;

    bool valid = true;
    for (auto& specific_class : genset->getSpecificClass()) {
        Class* child = (Class*) symbol_table.get(specific_class);

        if (child->getStereotype() != "role") {
            std::cout << "All specifics classes in Genset \"" << genset->getName() << "\" must be roles in RoleMixin pattern.\n";
            return;
        }

        if (
            child->getSuperClasses().size() != 2 || 
            (child->getSuperClasses().front() != mother_class && child->getSuperClasses().back() != mother_class)
        ) {
            valid = false; 
        }
    }
    
    if (!valid) {
        std::cout << "All roles in Genset \"" << genset->getName() << "\" must specialize a roleMixin and a class.\n";
        return;
    }
    
    printResult(RoleMixin, genset);
}

void SemanticAnalyzer::checkRelator(Class* cls) {
    if (cls->getStereotype() != "relator") return;

    bool has_material_relation = false;
    for (auto& child : cls->getChildren()) {
        if (auto relation = dynamic_cast<Relation*>(child)) {
            if (relation->getStereotype() != "mediation") {
                std::cout << "Every relation in Relator \"" << cls->getName() << "\" must be of type mediation.\n";
                return;
            }

            Class * related_class = (Class*) symbol_table.get(relation->getImage());
            for (auto& c : related_class->getChildren()) {
                if (auto rel = dynamic_cast<Relation*>(c)) {
                    if (rel->getStereotype() == "material")
                        has_material_relation = true;
                }
            }
        }
        else {
            std::cout << "Relator \"" << cls->getName() << "\" cannot have attributes.\n";
            return;
        }
    }

    if (!has_material_relation) {
        std::cout << "Relator \"" << cls->getName() << "\" does not have material relations.\n";
        return;
    }

    printResult(Relator, cls);
}

void SemanticAnalyzer::checkMode(Class* cls) {
    if (cls->getStereotype() != "mode") return;

    bool characterization = false;
    for (auto& child : cls->getChildren()) {
        if (auto relation = dynamic_cast<Relation*>(child)) {
            if (relation->getStereotype() == "characterization") {
                if (characterization) {
                    std::cout << "Mode \"" << cls->getName() << "\" must have exactly one characterization.\n";
                    return;
                }
                
                characterization = true;
            }
            else if (relation->getStereotype() != "externalDependence") {
                std::cout << "Every relation in Mode \"" << cls->getName() << "\" must be of type characterization or externalDependence.\n";
                return;
            }
        }
        else {
            std::cout << "Mode \"" << cls->getName() << "\" cannot have attributes.\n";
            return;
        }
    }

    if (!characterization) {
        std::cout << "Mode \"" << cls->getName() << "\" does not have a characterization relation.\n";
        return;
    }

    printResult(Mode, cls);
}

std::pair<bool, bool> checkGenset(Genset* genset, const std::string& stereotype, SymbolTable& st) {
    std::string mother_class = genset->getMotherClass();

    bool all_equal = true;
    bool valid = true;
    for (auto& specific_class : genset->getSpecificClass()) {
        Class* child = (Class*) st.get(specific_class);

        if (child->getStereotype() != stereotype) {
            all_equal = false;
            break;
        }

        if (child->getSuperClasses().size() != 1 || child->getSuperClasses().front() != mother_class) {
            valid = false; 
        }
    }
    
    if (all_equal && !valid) {
        std::cout << "All " << stereotype << "s in Genset \"" << genset->getName() << "\" must specialize only the general class.\n";
    }
    
    return { all_equal, valid };
}

void printResult(Pattern pattern, Node* node) {

}
