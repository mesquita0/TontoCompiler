#include "scanner.h"
#include <FlexLexer.h>
#include "token.h"
#include "parser.h"
#include <iostream>

Token Scanner::scan() {
    TokenClass token_class = (TokenClass) scanner.yylex();

    if (token_class == (TokenClass) INVALID) {
        std::cout << "Error at line " << scanner.lineno() << ", column " << yycolumn
                    << ". Token \"" << error_token << "\" is invalid." << std::endl;
    }

    return Token(token_class, scanner.YYText(), scanner.lineno());
}
