%define api.value.type { Token }

%code requires {
    #include "token.h"
}

%{
#include <stdio.h>
#include "scanner.h"

extern Token yylval;
extern int yylex();
extern int yyerror(const char *s);
%}

// Keywords
%token KW_GENSET
%token KW_DISJOINT
%token KW_COMPLETE
%token KW_GENERAL
%token KW_SPECIFICS
%token KW_WHERE
%token KW_PACKAGE
%token KW_IMPORT
%token KW_FUNCTIONAL_COMPLEXES
%token KW_AS
%token KW_DATATYPE
%token KW_ENUM
%token KW_RELATION
%token KW_INSTANCEOF
%token KW_OF
%token KW_OBJECTS
%token KW_TYPES
%token KW_ABSTRACT_INDIVIDUALS
%token KW_QUANTITIES
%token KW_RELATORS

// Symbols
%token SYM_LBRACE        // {
%token SYM_RBRACE        // }
%token SYM_LPAREN        // (
%token SYM_RPAREN        // )
%token SYM_LBRACKET      // [
%token SYM_RBRACKET      // ]
%token SYM_ASSOC_O       // <o>--
%token SYM_ASSOC_EMPTY   // <>--
%token SYM_ASSOC_EMPTY_R // --<>
%token SYM_DASH          // --
%token SYM_STAR          // *
%token SYM_AT            // @
%token SYM_COLON         // :
%token SYM_COMMA         // ,
%token SYM_DOT           // .
%token SYM_DOTDOT        // ..
%token SYM_EQUAL         // =


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
