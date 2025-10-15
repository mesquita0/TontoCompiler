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
            tokens.emplace_back((TokenClass) lookahead, scanner.YYText());
        else {
            std::cout << "Error at line " << scanner.lineno() << ". Token '"
                      << scanner.YYText() << "' is invalid." << std::endl;
            sucess = false;
            tokens.clear();
        }
    }

    return sucess;
}
