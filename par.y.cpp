%{
#include<stdio.h>
#include "ast.h"
#include <stdlib.h>
#include <string.h>
void yyerror(char*);
int n;
int col=0;
int lin=1;
int yylex();
%}

%token  Is Nullable In 
%token  Empty Epsilon Single symbole
%token  Finite  AST Print Infinite 
 
%left '|'
%left '*'

%union {
  struct ast * a;     
  char * chaine  ;          
       }
%token <chaine> word
%type <a> exp1 exp2 exp3 exp4        

%start input 

%%

input : {printf("> ");}
      |input line 
      ;
line : '\n' 
       | Is exp1 Nullable '?' '\n' {if(contient_epsilon($2)) puts("yes") ;
                                    else puts("No");
                                    printf("> ");
                                     n=0; lin=1; liberer_ast($2);  
                                    }
       | Is word In exp1 '?' '\n' {if(appartient($2,$4)) puts("yes") ;
                                     else puts("NO") ;
                                     n=0; lin=1; liberer_ast($4);
                                     printf("> ");  }
       | Is exp1 Empty '?' '\n' {if(est_vide($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  n=0; lin=1; liberer_ast($2); 
                                  printf("> "); 
                                  }
       | Is exp1 Epsilon '?' '\n' {if(est_epsilon($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  n=0; lin=1; liberer_ast($2); 
                                  printf("> "); 
                                  }  
         | Is exp1 Single '?' '\n' {if(est_single($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  n=0; lin=1; liberer_ast($2); 
                                  printf("> "); 
                                  }
        | Is exp1 Finite '?' '\n' {if(est_fini($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  n=0; lin=1; liberer_ast($2); 
                                  printf("> "); 
                                  } 
        | Is exp1 Infinite '?' '\n' {if(!est_fini($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  n=0; lin=1; liberer_ast($2); 
                                  printf("> "); 
                                  }  
        | Is exp1 symbole '?' '\n' {if(est_symbole($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  n=0; lin=1; liberer_ast($2); 
                                  printf("> "); 
                                  }                                                                                                                                            
        
       | Print AST exp1 '\n' { imprimer_ast($3);
                              printf("> ");  
                             indice=0; 
                              n=0; 
                              liberer_ast($3); 
                              } 
       | exp1 '\n' {
                             printf("expession valide la taille=%d\n",n);
                             n=0; 
                             liberer_ast($1); 
                             printf(" > nouveau  exp  s v p :");
                             lin=1;
                   }
      ;
exp1 : exp1 '|' {n++;} exp2 {$$=creer_union($1,$4);}
     | exp2 {$$=copier_ast($1); }
     ;
exp2 : exp2 {n++;} exp3 {$$=creer_concat($1,$3);}  
      | exp3 {$$=copier_ast($1);}     
exp3 : exp3 '*' {n++; $$=creer_etoile($1);}  
     | exp4 {$$=copier_ast($1);}
     ;

     ;   
exp4 : 'a' { n++; $$=creer_a();}
     | 'b' { n++; $$=creer_b();}
     | '1' { $$=creer_epsilon();}
     | '0' { $$=creer_vide();}
     | '(' exp1 ')' {  $$=copier_ast($2);}
     ;
%%
void yyerror(char* erreur){
     printf("%s,lin %d col %d ",erreur,lin,col);
     }
int main(){
    ;
    return yyparse();
    return 0;
}

                  
     
         
       
          
