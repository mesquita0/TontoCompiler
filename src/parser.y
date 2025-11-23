%define api.value.type { Token }

%{
#include <stdio.h>
#include "scanner.h"

extern Token yylval;
extern int yylex();
extern int yyerror(const char *s);
%}

%token RESERVED_KEYWORD
%token SYMBOL
%token CLASS_STEREOTYPE
%token CLASS_ID
%token RELATION_STEREOTYPE
%token RELATION_ID
%token INSTANCE_ID
%token NATIVE_DT
%token CUSTOM_DT
%token META_ATTRIBUTE
%token INVALID

%%

input:
    ;

%%

int yylex() {
    static Scanner scanner;

    yylval = scanner.scan(); 
    return yylval.tokenClass();
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
