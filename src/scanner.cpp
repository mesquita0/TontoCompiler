#include "scanner.h"
#include <FlexLexer.h>
#include <list>
#include "token.h"
#include <iostream>

bool Scanner::scan() {
    int lookahead;
    sucess = true;
    tokens = {};

    while ((lookahead = scanner.yylex()) != 0 && sucess) {
        if (lookahead != INVALID)
            tokens.emplace_back((TokenClass) lookahead, scanner.YYText(), scanner.lineno());
        else {
            std::cout << "Error at line " << scanner.lineno() << ", column " << yycolumn
                      << ". Token \"" << error_token << "\" is invalid." << std::endl;
            sucess = false;
            tokens.clear();
        }
    }

    return sucess;
}
