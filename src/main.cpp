#include "scanner.h"
#include <iostream>
#include <iomanip>
#include <string>

std::string class_str[] = {
	"",
	"Reserved Keyword",
	"Symbol",
	"Class Stereotype",
	"Class Identifier",
	"Relation Stereotype",
	"Relation Identifier",
	"Instance Identifier",
	"Native Datatype",
	"Custom Datatype",
	"Meta Attribute"
};

int main() {
	Scanner scanner = {};
	bool sucess = scanner.scan();

	if (sucess) {
		int counts[11] = {};

		std::cout << "Token List" << std::endl;

		for (const Token& token : scanner) {
			++counts[token.tokenClass()];

			std::cout << std::left << std::setw(19) << class_str[token.tokenClass()] << ": \"" 
					  << token.Lexeme() << "\" at line " << token.LineNum()
					  << ", column " << token.ColumnNum() << "." << std::endl;		
		}

		std::cout << std::endl;

		std::cout << "Synthesis Table" << std::endl;

		for (int i = 1; i < 11; i++) {
			if (counts[i] == 0) continue;

			std::cout << std::left << std::setw(19) << class_str[i] << ": " << counts[i]
					  << (counts[i] > 1 ? " tokens" : " token") << std::endl;
		}
	}
}
