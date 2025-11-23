#include "scanner.h"
#include <FlexLexer.h>
#include <list>
#include "token.h"
#include <iostream>

Token Scanner::scan() {
    TokenClass::T token_class = (TokenClass::T) scanner.yylex();

    if (token_class == TokenClass::INVALID) {
        std::cout << "Error at line " << scanner.lineno() << ", column " << yycolumn
                    << ". Token \"" << error_token << "\" is invalid." << std::endl;
    }

    return Token(token_class, scanner.YYText(), scanner.lineno());
}
