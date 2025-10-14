%{
#include "tokens.h"
%}

%option noyywrap c++

delim        [ \t\n]
ws           {delim}+
class_id     [A-Z][A-Za-z_]+
relation_id  [a-z][A-Za-z_]+
instance_id  [A-Za-z_]+[0-9]+
custom_dt    [A-Za-z]+DataType

%%
{ws}          ;

genset        return RESERVED_KEYWORD;
disjoint      return RESERVED_KEYWORD;
complete      return RESERVED_KEYWORD;
general       return RESERVED_KEYWORD;
specifics     return RESERVED_KEYWORD;
where         return RESERVED_KEYWORD;
package       return RESERVED_KEYWORD;

 /* TODO: CLASS_STEREOTYPE */

 /* TODO: RELATION_STEREOTYPE */

number        return NATIVE_DT;
string        return NATIVE_DT;
boolean       return NATIVE_DT;
date          return NATIVE_DT;
time          return NATIVE_DT;
datetime      return NATIVE_DT;

ordered       return META_ATTRIBUTE;
const         return META_ATTRIBUTE;
derived       return META_ATTRIBUTE;
subsets       return META_ATTRIBUTE;
redefines     return META_ATTRIBUTE;

{custom_dt}   return CUSTOM_DT;
{class_id}    return CLASS_ID;
{relation_id} return RELATION_ID;
{instance_id} return INSTANCE_ID;

"{"           return SYMBOL;
"}"           return SYMBOL;
"("           return SYMBOL;
")"           return SYMBOL;
"["           return SYMBOL;
"]"           return SYMBOL;
".."          return SYMBOL;
"<>--"        return SYMBOL;
"--<>"        return SYMBOL;
"*"           return SYMBOL;
"@"           return SYMBOL;
":"           return SYMBOL;

.             return INVALID;

%%
