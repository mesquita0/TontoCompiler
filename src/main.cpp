#include "AST.h"
#include "SymbolTable.h"
#include "parser.h"
#include "scanner.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

extern FILE* yyin;

int main(int argc, char** argv) {
	if (argc < 2) {
        std::cerr << "Usage: TontoCompiler <file1> [file2 ...]\n";
        return 1;
    }

	SymbolTable symbol_table = SymbolTable();
	AST ast = AST(symbol_table);
	
	for (int i = 1; i < argc; i++)
    {
		const char* filename = argv[i];

		std::ifstream input(filename);
		if (!input.good()) {
            std::cerr << "Cannot open: " << filename << "\n";
            continue;
        }
		
		Scanner scanner = Scanner(input);

        yyparse(&ast, scanner);
    }

	ast.print_summary();
}
