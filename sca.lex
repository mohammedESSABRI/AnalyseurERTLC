%{

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "par.tab.h"

void yyerror(char* );

%}

%%

"Is"       return Is;
"Nullable"   return Nullable;
"In"        return In;
"Empty"     return Empty;
"Epsilon"   return Epsilon;
"Single"     return Single;
"Finite"      return Finite;
"Infinite"    return Infinite;
"Print"       return Print;
"AST"         return AST;
"symbole"      return symbole;
"hlp"          return hlp;
"dnc" dnc();//deconnexion
"qt" return qt;
a |  
b |
0 |
1 |
"(" | 
")" |
"*" |
"|" |
"?"      {return *yytext;}
\n           {return *yytext; }  
[ ]+          { ; }
"<"[ab1]+">"   {
              yylval.chaine=strdup(++yytext);//ignorer le "<" et le ">"
              yylval.chaine[strlen(yylval.chaine) -1] = '\0';
              return word;
              }
.   {yyerror("\nerreur lexicale");brrr(); }
%%




       
 
