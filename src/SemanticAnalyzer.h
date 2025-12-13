#include "AST.h"
#include "SymbolTable.h"

class SemanticAnalyzer {
    AST& ast;
    SymbolTable& symbol_table;

public:
    SemanticAnalyzer(AST& ast, SymbolTable& st): ast(ast), symbol_table(st) { }

    void analyze();
};
