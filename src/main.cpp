#include "scanner.h"
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
	Scanner scanner = {};
	bool sucess = scanner.scan();

	if (sucess) {
		for (const Token& token : scanner) {
			std::string token_class;

			switch (token.tokenClass())
			{
				case RESERVED_KEYWORD:    token_class = "RESERVED_KEYWORD"; break;
				case SYMBOL: 		      token_class = "SYMBOL"; break;
				case CLASS_STEREOTYPE:    token_class = "CLASS_STEREOTYPE"; break;
				case CLASS_ID:            token_class = "CLASS_ID"; break;
				case RELATION_STEREOTYPE: token_class = "RELATION_STEREOTYPE"; break;
				case RELATION_ID: 		  token_class = "RELATION_ID"; break;
				case INSTANCE_ID: 		  token_class = "INSTANCE_ID"; break;
				case NATIVE_DT:           token_class = "NATIVE_DT"; break;
				case CUSTOM_DT:           token_class = "CUSTOM_DT"; break;
				case META_ATTRIBUTE:      token_class = "META_ATTRIBUTE"; break;
			}

			std::cout << std::left << std::setw(19) << token_class << ": \"" 
					  << token.Lexeme() << "\" at line " << token.LineNum()
					  << ", column " << token.ColumnNum() << "." << std::endl;
		}
	}
}
