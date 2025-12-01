#include "scanner.h"
#include "token.h"
#include "parser.h"
#include <FlexLexer.h>
#include <fstream>
#include <iostream>

Scanner::Scanner(std::ifstream& stream) {
    scanner.switch_streams(&stream, nullptr);
}

Token Scanner::scan() {
    TokenClass token_class = (TokenClass) scanner.yylex();

    if (token_class == (TokenClass) INVALID) {
        std::cout << "Error at line " << scanner.lineno() << ", column " << yycolumn
                    << ". Token \"" << error_token << "\" is invalid." << std::endl;
    }

    return Token(token_class, scanner.YYText(), scanner.lineno());
}
