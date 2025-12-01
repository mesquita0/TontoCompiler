#include "AST.h"
#include "SymbolTable.h"
#include "parser.h"
#include <iostream>
#include <iomanip>
#include <string>

int main() {
	SymbolTable symbol_table = SymbolTable();
	AST ast = AST(symbol_table);

	yyparse(&ast);

	ast.print_summary();
}
