#pragma once
#include <string>

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

public:
    Token(TokenClass tc, std::string lexeme) : token_class(tc), lexeme(lexeme) {}

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
};
