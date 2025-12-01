#pragma once
#include "token.h"
#include <FlexLexer.h>
#include <fstream>

class Scanner {
	yyFlexLexer scanner;
	
public:
	Scanner() = default;
	Scanner(std::ifstream& stream);

	Token scan();
};
