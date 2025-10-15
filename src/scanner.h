#pragma once
#include <FlexLexer.h>
#include <list>
#include "token.h"

class Scanner {
	yyFlexLexer scanner;
    std::list<Token> tokens;
    bool sucess = false;
	
public:
	bool scan();

    auto begin() const { return tokens.begin(); }
    auto end() const { return tokens.end(); }
};
