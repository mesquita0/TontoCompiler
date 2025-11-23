#pragma once
#include <FlexLexer.h>
#include <list>
#include "token.h"

class Scanner {
	yyFlexLexer scanner;
	
public:
	Token scan();
};
