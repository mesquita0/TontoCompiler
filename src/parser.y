%define api.value.type { Token }

%code requires {
    #include "AST.h"
    #include "token.h"
}

%{
#include "AST.h"
#include "scanner.h"
#include <stdio.h>
#include <string>

// Variables for ast
Node* current_package;
Node* current_class;
Node* current_type;
Node* current_enum;
Node* current_attribute_context;
Genset* current_genset;

bool is_internal_relation;
std::string relation_id;

extern Token yylval;
extern int yylex();
extern int yyerror(AST* ast, const char *s);
%}

%parse-param { AST* ast }

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
%token KW_SPECIALIZES

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


%token INT
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

tonto_model:
        package_decl import_decl declarations
    ;

package_decl:
        KW_PACKAGE CLASS_ID
        {
            current_package = ast->add_package($2.Lexeme());
        }
    ;

import_decl:    
        import_decl KW_IMPORT CLASS_ID
    |
    ;

declarations:
        declarations class_decl
    |   declarations type_decl
    |   declarations enum_decl
    |   declarations gen_decl
    |   declarations ex_relation_decl
    |     
    ;


// Class declaration
class_decl:
        CLASS_STEREOTYPE CLASS_ID specializes_decl class_body
        {
            current_class = ast->add_class((Package*) current_package, $2.Lexeme());
            current_attribute_context = current_class;
        }
    ;

specializes_decl:
        KW_SPECIALIZES classes
    |
    ;

classes:
        CLASS_ID SYM_COMMA classes
    |   CLASS_ID
    ;

class_body:
        SYM_LBRACE class_elements SYM_RBRACE
    |   SYM_LPAREN instance_decl SYM_RPAREN
    |
    ;

class_elements:
        class_elements attribute_decl
    |   class_elements in_relation_decl
    |   attribute_decl  
    |   in_relation_decl 
    ;

attribute_decl:
        CLASS_ID SYM_COLON type meta_attribute
        {
            ast->add_attribute(current_attribute_context, $1.Lexeme(), $3.Lexeme());
        }
    ;

type:
        NATIVE_DT
    |   CUSTOM_DT
    ;

instance_decl:
        KW_INSTANCEOF CLASS_ID
    ;

meta_attribute:
        SYM_LBRACE META_ATTRIBUTE SYM_RBRACE
    |
    ;


// Type declaration
type_decl:
        KW_DATATYPE CLASS_ID SYM_LBRACE datatype_elements SYM_RBRACE
        {
            current_type = ast->add_type((Package*) current_package, $1.Lexeme());
            current_attribute_context = current_type;
        }
    ;

datatype_elements:
        datatype_elements attribute_decl
    |   attribute_decl
    |   
    ;


// Enum declaration
enum_decl:
        KW_ENUM CLASS_ID SYM_LBRACE enum_elements SYM_RBRACE
        {
            current_enum = ast->add_enum((Package*) current_package, $2.Lexeme());
        }
    ;

enum_elements:
        enum_element SYM_COMMA enum_elements
    |   enum_element   
    ;

enum_element:
        INSTANCE_ID
        {
            ast->add_instance((Enum*) current_enum, $1.Lexeme());
        }
    ;


// Genelarization declaration
gen_decl:
        opt_disjoint opt_complete KW_GENSET CLASS_ID gen_body
        {
            current_genset = (Genset*) ast->add_genset((Package*) current_package, $4.Lexeme());
        }
    ;

opt_disjoint:
        KW_DISJOINT
    |
    ;

opt_complete:
        KW_COMPLETE
    |
    ;

gen_body:
        gen_where
    |   SYM_LBRACKET gen_elements SYM_RBRACKET
    ;

gen_elements:
        gen_elements gen_element
    |   gen_element
    ;

gen_element:
        KW_GENERAL CLASS_ID
        {
            current_genset->setMotherClass($2.Lexeme());
        }
    |   KW_SPECIFICS class_list
    ;

gen_where:
        KW_WHERE class_list KW_SPECIALIZES CLASS_ID
         {
            current_genset->setMotherClass($4.Lexeme());
        }
    |
    ;

class_list:
        class_name SYM_COMMA class_list
    |   class_name
    ;

class_name:
        CLASS_ID
        {
            current_genset->addClass($1.Lexeme());
        }
    ;


// Internal relation declaration
in_relation_decl:
        SYM_AT RELATION_STEREOTYPE relation_body
        {
            is_internal_relation = true;
        }
    ;

relation_body:
        cardinality relation_connector cardinality CLASS_ID
        {
            if (is_internal_relation) {
                ast->add_relation((Class*) current_class, $4.Lexeme());
            }
            else {
                ast->add_relation((Package*) current_package, relation_id, $4.Lexeme());
            }
        }
    ;

cardinality:
        SYM_LBRACKET cardinality_bound cardinality_range SYM_LBRACKET
    |
    ;

cardinality_bound:
        INT
    |   SYM_STAR   
    ;

cardinality_range:
        SYM_DOTDOT cardinality_bound
    |
    ;

relation_connector:
        SYM_ASSOC_O
    |   SYM_ASSOC_EMPTY
    |   SYM_ASSOC_EMPTY_R
    |   SYM_DASH
    ;


// External relation declaration
ex_relation_decl:
        SYM_AT RELATION_STEREOTYPE RELATION_ID relation_body 
        {
            is_internal_relation = false;
            relation_id = $3.Lexeme();
        }
    ;

%%

int yylex() {
    static Scanner scanner;

    yylval = scanner.scan(); 
    return yylval.tokenClass();
}

int yyerror(AST* ast, const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
