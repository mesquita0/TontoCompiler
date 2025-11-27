#include "AST.h"
#include "parser.h"
#include <iostream>
#include <iomanip>
#include <string>

int main() {
	AST ast = AST();

	yyparse(&ast);
}
