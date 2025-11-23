#pragma once
#include <FlexLexer.h>
#include "token.h"

class Scanner {
	yyFlexLexer scanner;
	
public:
	Token scan();
};
