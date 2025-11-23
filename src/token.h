#pragma once
#include <string>

extern int yycolumn;
extern std::string error_token;

namespace TokenClass {
    enum T {
        RESERVED_KEYWORD = 1,
        SYMBOL,
        CLASS_STEREOTYPE,
        CLASS_ID,
        RELATION_STEREOTYPE,
        RELATION_ID,
        INSTANCE_ID,
        NATIVE_DT,
        CUSTOM_DT,
        META_ATTRIBUTE,
        INVALID
    };
}

class Token {
    TokenClass::T  token_class;
    std::string lexeme; 
    int line, column;

public:
    Token() : token_class(TokenClass::INVALID), lexeme(""), line(0), column(0) {}

    Token(TokenClass::T tc, std::string lexeme, int line)
     : token_class(tc), lexeme(lexeme), line(line) { 
        column = yycolumn;
    }

    TokenClass::T tokenClass() const {
        return token_class;
    }

    void tokenClass(TokenClass::T token_class) {
        this->token_class = token_class;
    }

    std::string Lexeme() const {
        return lexeme;
    }

    void Lexeme(std::string lexeme) {
        this->lexeme = lexeme;
    }

    int LineNum() const {
        return line;
    }

    int ColumnNum() const {
        return column;
    }
};
