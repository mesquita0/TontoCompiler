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
event        return CLASS_STEREOTYPE;
situation    return CLASS_STEREOTYPE;
process      return CLASS_STEREOTYPE;
category     return CLASS_STEREOTYPE;
mixin        return CLASS_STEREOTYPE;
phaseMixin   return CLASS_STEREOTYPE;
roleMixin    return CLASS_STEREOTYPE;
historicalRoleMixin  return CLASS_STEREOTYPE;
kind         return CLASS_STEREOTYPE;
collective   return CLASS_STEREOTYPE;
quantity     return CLASS_STEREOTYPE;
quality      return CLASS_STEREOTYPE;
mode         return CLASS_STEREOTYPE;
intrinsicMode  return CLASS_STEREOTYPE;
extrinsicMode  return CLASS_STEREOTYPE;
subkind      return CLASS_STEREOTYPE;
phase        return CLASS_STEREOTYPE;
role         return CLASS_STEREOTYPE;
historicalRole  return CLASS_STEREOTYPE;

 
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
