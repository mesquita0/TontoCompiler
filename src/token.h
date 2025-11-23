#pragma once
#include <string>

extern int yycolumn;
extern std::string error_token;

enum TokenClass : int;

class Token {
    TokenClass token_class;
    std::string lexeme; 
    int line, column;

public:
    Token() : token_class((TokenClass) 0), lexeme(""), line(0), column(0) {}

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
