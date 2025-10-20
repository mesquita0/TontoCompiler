#pragma once
#include <string>

extern int yycolumn;

enum TokenClass {
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

class Token {
    TokenClass  token_class;
    std::string lexeme; 
    int line, column;

public:
    Token(TokenClass tc, std::string lexeme, int line)
     : token_class(tc), lexeme(lexeme), line(line) { 
        column = yycolumn;
    }

    TokenClass tokenClass() const {
        return token_class;
    }

    void tokenClass(TokenClass token_class) {
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
