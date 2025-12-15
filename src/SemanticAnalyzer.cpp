#include "SemanticAnalyzer.h"
#include "AST.h"
#include "SymbolTable.h"
#include <iostream>
#include <utility>
#include <string>
#include <list>

enum Pattern { 
    Subkind, Role, Phase, Relator, Mode, RoleMixin 
};

enum class Stereotype {
    Kind, Subkind, Phase, Role,
    Category, RoleMixin, Mixin,
    Relator, Mode, Invalid
};

Stereotype getStereotypeEnum(const std::string& s) {
    if (s == "kind") return Stereotype::Kind;
    if (s == "subkind") return Stereotype::Subkind;
    if (s == "phase") return Stereotype::Phase;
    if (s == "role") return Stereotype::Role;
    if (s == "category") return Stereotype::Category;
    if (s == "roleMixin") return Stereotype::RoleMixin;
    if (s == "mixin") return Stereotype::Mixin;
    if (s == "relator") return Stereotype::Relator;
    if (s == "mode") return Stereotype::Mode;
    return Stereotype::Invalid;
}

std::pair<bool, bool> checkGenset(Genset* genset, const std::string& stereotype, SymbolTable& st);
void printResult(Pattern pattern, Node* node);

void SemanticAnalyzer::analyze() {
    ast.consolidate();
    
    if (!checkReferences()) return;

    for (auto& [name, node] : symbol_table) {
        if (auto* genset = dynamic_cast<Genset*>(node)) {
            if (!checkGeneralizationCompatibility(genset))
                continue;

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

bool SemanticAnalyzer::checkReferences() {
    bool valid = true;

    for (auto& [name, node] : symbol_table) {
        if (auto* genset = dynamic_cast<Genset*>(node)) {
            // Mother class
            const std::string& mother = genset->getMotherClass();
            if (!mother.empty() && !symbol_table.get(mother)) {
                std::cout << "Undefined class referenced as general class \"" << mother << "\" in Genset \"" << genset->getName() << "\".\n";
                valid = false;
            }

            // Specific classes
            for (const auto& spec : genset->getSpecificClass()) {
                if (!symbol_table.get(spec)) {
                    std::cout << "Undefined class referenced as specific class \"" << spec << "\" in Genset \"" << genset->getName() << "\".\n";
                    valid = false;
                }
            }
        }
        else if (auto* cls = dynamic_cast<Class*>(node)) {
            // Superclasses
            for (const auto& super : cls->getSuperClasses()) {
                if (!symbol_table.get(super)) {
                    std::cout << "Undefined superclass \"" << super << "\" referenced by class \"" << cls->getName() << "\".\n";
                    valid = false;
                }
            }

            // Relations
            for (auto& child : cls->getChildren()) {
                if (auto* rel = dynamic_cast<Relation*>(child)) {
                    // Image class
                    if (!symbol_table.get(rel->getImage())) {
                        std::cout << "Undefined class \"" << rel->getImage() << "\" referenced in relation of class \"" << cls->getName() << "\".\n";
                        valid = false;
                    }

                    // Domain class
                    if (!rel->isInternal()) {
                        if (!symbol_table.get(rel->getDomain())) {
                            std::cout << "Undefined domain class \"" << rel->getDomain() << "\" referenced in external relation.\n";
                            valid = false;
                        }
                    }
                }
            }
        }
    }

    return valid;
}

bool SemanticAnalyzer::checkGeneralizationCompatibility(Genset* genset) {
    std::string generalName = genset->getMotherClass();
    Class* general = (Class*) symbol_table.get(generalName);

    if (!general) return true;

    Stereotype genType = getStereotypeEnum(general->getStereotype());

    for (const auto& specName : genset->getSpecificClass()) {
        Class* spec = (Class*) symbol_table.get(specName);
        if (!spec) continue;

        Stereotype specType = getStereotypeEnum(spec->getStereotype());

        bool valid = true;
        std::string error;

        switch (genType) {
            case Stereotype::Kind:
                valid = (specType == Stereotype::Subkind ||
                         specType == Stereotype::Phase ||
                         specType == Stereotype::Role
                        );
                error = "A Kind can only be specialized by Subkind, Phase, or Role.";
                break;

            case Stereotype::Subkind:
                valid = (specType == Stereotype::Subkind ||
                         specType == Stereotype::Phase ||
                         specType == Stereotype::Role
                        );
                error = "A Subkind can only be specialized by Subkind, Phase, or Role.";
                break;

            case Stereotype::Phase:
                valid = (specType == Stereotype::Phase ||
                         specType == Stereotype::Role
                        );
                error = "A Phase can only be specialized by Phase or Role.";
                break;

            case Stereotype::Role:
                valid = (specType == Stereotype::Role);
                error = "A Role can only be specialized by another Role.";
                break;

            case Stereotype::Category:
                valid = (specType == Stereotype::Kind ||
                         specType == Stereotype::Subkind ||
                         specType == Stereotype::Category
                        );
                error = "A Category can only be specialized by Kind, Subkind, or Category.";
                break;

            case Stereotype::RoleMixin:
                valid = (specType == Stereotype::Role ||
                         specType == Stereotype::RoleMixin
                        );
                error = "A RoleMixin can only be specialized by Role or RoleMixin.";
                break;

            case Stereotype::Mixin:
                valid = (specType == Stereotype::Mixin ||
                         specType == Stereotype::Category ||
                         specType == Stereotype::RoleMixin ||
                         specType == Stereotype::Kind ||
                         specType == Stereotype::Subkind ||
                         specType == Stereotype::Phase ||
                         specType == Stereotype::Role
                        );
                error = "A Mixin can only be specialized by Mixins or Sortals.";
                break;

            default:
                break;
        }

        if (!valid) {
            std::cout << "Invalid generalization in Genset \"" << genset->getName() << "\": "
                      << specName << " -> " << generalName << ". " << error << "\n";
            return false;
        }
    }

    return true;
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
    
    std::string patternName;
    std::string details = "";

    switch (pattern) {

        case Subkind:
        case Role:
        case Phase:
        case RoleMixin: 
        {
            if (pattern == Subkind) patternName = "Subkind";
            else if (pattern == Role) patternName = "Role";
            else if (pattern == Phase) patternName = "Phase";
            else patternName = "RoleMixin";

            if (auto* genset = dynamic_cast<Genset*>(node)) {
             
                details += "\nGeneral: " + genset->getMotherClass();
                
                details += "\nSpecifics: [";
                
                auto specifics = genset->getSpecificClass(); 
                bool first = true;
                for (const auto& spec : specifics) {
                    if (!first) details += ", ";
                    details += spec;
                    first = false;
                }
                
                details += "]";
            }
            break;
        }

        case Relator:
        case Mode:
        {
            patternName = (pattern == Relator) ? "Relator" : "Mode";

       
            if (auto* cls = dynamic_cast<Class*>(node)) {
                details += "\nRelations: ";
                bool first = true;
                
                for (auto& child : cls->getChildren()) {
                    if (auto* relation = dynamic_cast<Relation*>(child)) {
                        if (!first) details += ", ";
                        details += "(" + relation->getStereotype() + " -> " + relation->getImage() + ")";
                        first = false;
                    }
                }
            }
            break;
        }

        default: 
            patternName = "Unknown"; 
            break;
    }

    std::cout << "Pattern Identified: " << patternName << "\n"
              << "Element: " << node->getName()
              << details  
              << "\n"     
              << std::endl;
}
