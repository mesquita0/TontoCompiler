#include "AST.h"
#include "SymbolTable.h"

class SemanticAnalyzer {
    AST& ast;
    SymbolTable& symbol_table;

    bool checkReferences();

    bool checkGeneralizationCompatibility(Genset* genset);
    void checkSubkind(Genset* genset);
    void checkRole(Genset* genset);
    void checkPhase(Genset* genset);
    void checkRoleMixin(Genset* genset);

    void checkRelator(Class* cls);
    void checkMode(Class* cls);

public:
    SemanticAnalyzer(AST& ast, SymbolTable& st): ast(ast), symbol_table(st) { }

    void analyze();
};
