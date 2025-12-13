%define api.value.type { Token }
%define parse.error verbose

%code requires {
    #include "AST.h"
    #include "token.h"

    class Scanner;
}

%{
#include "AST.h"
#include "scanner.h"
#include <stdio.h>
#include <string>

// Variables for ast
Node* current_package = nullptr;
Node* current_class = nullptr;
Node* current_type = nullptr;
Node* current_enum = nullptr;
Node* current_attribute_context = nullptr;
Genset* current_genset = nullptr;

std::string related_class = "";

bool genset_disjoint = false;
bool genset_complete = false;

int yylineno = 1;
extern int yycolumn;
extern std::string error_token;

extern Token yylval;
extern int yylex(Scanner& scanner);
extern int yyerror(AST* ast, Scanner& scanner, const char *s);
%}

%parse-param { AST* ast }
%parse-param { Scanner& scanner }
%lex-param   { Scanner& scanner }

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
    |   declarations error SYM_RBRACE { yyerrok; }  
    |     
    ;


// Class declaration
class_decl:
        CLASS_STEREOTYPE CLASS_ID
        {
            if (current_package) {
                current_class = ast->add_class((Package*) current_package, $1.Lexeme(), $2.Lexeme());
                current_attribute_context = current_class;
            }
        }
        specializes_decl class_body
    ;

specializes_decl:
        KW_SPECIALIZES classes
    |
    ;

classes:
        super_class_name SYM_COMMA classes
    |   super_class_name
    ;

super_class_name:
        CLASS_ID
    {
        if (current_class)
            ((Class*) current_class)->addSuperClass($1.Lexeme());
    }

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
        type_id SYM_COLON type meta_attribute
        {
            if (current_attribute_context) 
                ast->add_attribute(current_attribute_context, $1.Lexeme(), $3.Lexeme());
        }
    ;

type_id:
        CLASS_ID
    |   RELATION_ID
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
        KW_DATATYPE CLASS_ID 
        {
            if (current_package) {
                current_type = ast->add_type((Package*) current_package, $2.Lexeme());
                current_attribute_context = current_type;
            }
        }
        SYM_LBRACE datatype_elements SYM_RBRACE
    ;

datatype_elements:
        datatype_elements attribute_decl
    |   attribute_decl
    |   
    ;


// Enum declaration
enum_decl:
        KW_ENUM CLASS_ID 
        {
            if (current_package)
                current_enum = ast->add_enum((Package*) current_package, $2.Lexeme());
        }
        SYM_LBRACE enum_elements SYM_RBRACE
    ;

enum_elements:
        enum_element SYM_COMMA enum_elements
    |   enum_element   
    ;

enum_element:
        INSTANCE_ID
        {
            if (current_enum)
                ast->add_instance((Enum*) current_enum, $1.Lexeme());
        }
    ;


// Genelarization declaration
gen_decl:
        opt_disjoint opt_complete KW_GENSET CLASS_ID 
        {
            if (current_package) {
                current_genset = (Genset*) ast->add_genset((Package*) current_package, $4.Lexeme());
                if (genset_disjoint) current_genset->setDisjoint();
                if (genset_complete) current_genset->setComplete();
            }
        }
        gen_body
    ;

opt_disjoint:
        KW_DISJOINT { genset_disjoint = true; }
    |   { genset_disjoint = false; }
    ;

opt_complete:
        KW_COMPLETE { genset_complete = true; }
    |   { genset_complete = false; }
    ;

gen_body:
        gen_where
    |   SYM_LBRACE gen_elements SYM_RBRACE
    ;

gen_elements:
        gen_elements gen_element
    |   gen_element
    ;

gen_element:
        KW_GENERAL CLASS_ID
        {
            if (current_genset)
                current_genset->setMotherClass($2.Lexeme());
        }
    |   KW_SPECIFICS class_list
    ;

gen_where:
        KW_WHERE class_list KW_SPECIALIZES CLASS_ID
         {
            if (current_genset)
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
            if (current_genset)
                current_genset->addClass($1.Lexeme());
        }
    ;


// Internal relation declaration
in_relation_decl:
        SYM_AT RELATION_STEREOTYPE relation_body
        {
            if (current_class)
                ast->add_relation((Class*) current_class, $2.Lexeme(), related_class);
        }
    ;

relation_body:
        cardinality relation_connector cardinality CLASS_ID
        {
            related_class = $4.Lexeme();
        }
    ;

cardinality:
        SYM_LBRACKET cardinality_bound cardinality_range SYM_RBRACKET
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
        SYM_AT RELATION_STEREOTYPE KW_RELATION CLASS_ID relation_body
        {
            if (current_package) 
                ast->add_relation((Package*) current_package, $2.Lexeme(), $4.Lexeme(), related_class);
        }
    ;

%%

int yylex(Scanner& scanner) {
    yylval = scanner.scan();
    yylineno = yylval.LineNum();
    yycolumn = yylval.ColumnNum();
   
    try {
        error_token = yylval.Lexeme();
    } catch (...) {
        error_token = "<unknown>";
    }

    yycolumn = yylval.ColumnNum();

    return yylval.tokenClass();
}

static std::string suggest_fix(const std::string& unexpected, const char* msg) {
    std::string m(msg);

    // package
    if (m.find("expecting KW_PACKAGE") != std::string::npos) {
        return "Every Tonto file must start with a 'package PackageName' declaration.";
    }

    // atributo
    if (m.find("unexpected RELATION_ID") != std::string::npos && 
        (m.find("expecting SYM_AT") != std::string::npos || m.find("expecting CLASS_ID") != std::string::npos)) {
        return "Looks like an attribute declaration (" + unexpected + "). Missing ':' before the type, or check if it represents an internal relation.";
    }

    // datatype
    if (m.find("unexpected CUSTOM_DT") != std::string::npos) {
        return "Unknown type. If this is a custom type, ensure it is declared with 'datatype Name { ... }' before use.";
    }

    // bloco
    if (unexpected == "{") return "Unexpected '{'. Block opened too early, or missing class/relation name?";
    if (unexpected == "}") return "Unexpected '}'. Block closed prematurely?";

    //conectores
    if (unexpected == "<o>--" || unexpected == "<>--" || unexpected == "--<>" || unexpected == "--")
        return "Relation connector out of context. Check syntax: @stereotype [card] connector [card] Class.";

    // classes e esteriótipos
    if (m.find("unexpected CLASS_ID") != std::string::npos) {
        if (m.find("expecting KW_RELATION") != std::string::npos)
             return "Missing 'relation' keyword after the relation stereotype (@mediation, @material, etc).";
        
        if (m.find("expecting KW_GENERAL") != std::string::npos)
             return "Inside a genset, expected 'general' keyword to define the parent class.";

        return "Unexpected class name here. Missing a stereotype before? (Ex: kind, phase, role, subkind, category).";
    }
 
    // enum
    if (m.find("unexpected INSTANCE_ID") != std::string::npos) {
        return "Instance found outside an enum? Verify if it is inside an 'enum Name { ... }' block.";
    }

    // genset
    if (m.find("expecting KW_SPECIFICS") != std::string::npos) {
        return "Incomplete genset declaration. After 'general Class', expected 'specifics' followed by subclasses.";
    }

    if (m.find("expecting") != std::string::npos) {
        if (m.find("SYM_RBRACE") != std::string::npos) return "Missing closing brace '}'.";
        if (m.find("SYM_LBRACE") != std::string::npos) return "Missing opening brace '{'.";
        if (m.find("CLASS_ID") != std::string::npos) return "Expected a Class or Type identifier.";
        if (m.find("RELATION_ID") != std::string::npos) return "Expected an attribute name (lowercase).";
        if (m.find("META_ATTRIBUTE") != std::string::npos) return "Malformed meta-attribute { ... }.";
        if (m.find("SYM_COLON") != std::string::npos) return "Missing ':' to separate name and type.";
    }

    return "Syntax error. Check the structure near this token.";
}

int yyerror(AST* ast, Scanner& scanner, const char *s) {
    std::string suggestion = suggest_fix(error_token, s);

    fprintf(stderr,
        "[Line %d, Column %d]\n"
        "Syntax error near '%s': %s\n"
        "Suggestion: %s\n\n",
        yylineno,
        yycolumn,
        error_token.c_str(),
        s,
        suggestion.c_str()
    );

    return 0;
}
