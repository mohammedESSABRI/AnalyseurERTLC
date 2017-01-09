%{
#include<stdio.h>
#include "ast.h"
#include <stdlib.h>
#include <string.h>
void yyerror(char*);
int taille;//calculer la taille d'une er


int yylex();
%}

%token  Is Nullable In 
%token  Empty Epsilon Single symbole
%token  Finite  AST Print Infinite 
%token  hlp qt
 
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

input : {acc();
        brrr();//le nom de l'usr+vide ligne+barre
        }
      |input line 
      ;
line : '\n'
       | qt {exit(0);}
       | hlp                {
                            FILE* fp=fopen("data\\hlp\\intrr.txt","r");
                            if(fp){
                                   int aaal='0';
                                   if(fp){
                                          
                                   while(aaal!=EOF)
                                   {aaal=fgetc(fp);
                                   putchar(aaal);
                                   }
                                   }
                            
                                   }
                             printf("\nsds");      
                            brrr();
                            
                           }
                             
       | Is exp1 Nullable '?' '\n' {if(contient_epsilon($2)) puts("yes") ;
                                    else puts("No");
                                    brrr();//vider la ligne + barre de choix
                                    taille=0;  liberer_ast($2);  
                                    }
       | Is word In exp1 '?' '\n' {if(appartient($2,$4)) puts("yes") ;
                                     else puts("NO") ;
                                     taille=0; liberer_ast($4);
                                     brrr();  }
       | Is exp1 Empty '?' '\n' {if(est_vide($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  taille=0;  liberer_ast($2); 
                                  brrr();
                                  }
       | Is exp1 Epsilon '?' '\n' {if(est_epsilon($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  taille=0;  liberer_ast($2); 
                                  brrr(); 
                                  }  
         | Is exp1 Single '?' '\n' {if(est_single($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  taille=0;  liberer_ast($2); 
                                  brrr();
                                  }
        | Is exp1 Finite '?' '\n' {if(est_fini($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  taille=0;  liberer_ast($2); 
                                  brrr();
                                  } 
        | Is exp1 Infinite '?' '\n' {if(!est_fini($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  taille=0; liberer_ast($2); 
                                  brrr(); 
                                  }  
        | Is exp1 symbole '?' '\n' {if(est_symbole($2)) puts("yes") ; 
                                  else puts("NO") ;
                                  taille=0;  liberer_ast($2); 
                                  brrr();
                                  }                                                                                                                                            
        
       | Print AST exp1 '\n' { imprimer_ast($3);
                              brrr();  
                             i=0; 
                              taille=0; 
                              liberer_ast($3); 
                              } 
       
       | exp1 '\n' {
                             printf("expession valide la taille=%d\n",taille);
                             taille=0; 
                             liberer_ast($1); 
                             brrr();
                            
                   }
      ;
exp1 : exp1 '|' {taille++;} exp2 {$$=creer_union($1,$4);}
     | exp2 {$$=copier_ast($1); }
     ;
exp2 : exp2 {taille++;} exp3 {$$=creer_concat($1,$3);}  
      | exp3 {$$=copier_ast($1);}     
exp3 : exp3 '*' {taille++; $$=creer_etoile($1);}  
     | exp4 {$$=copier_ast($1);}
     ;

     ;   
exp4 : 'a' { taille++; $$=creer_a();}
     | 'b' { taille++; $$=creer_b();}
     | '1' { $$=creer_epsilon();}
     | '0' { $$=creer_vide();}
     | '(' exp1 ')' {  $$=copier_ast($2);}
     ;
%%
void yyerror(char* erreur){
     puts(erreur);
     
     }
int main(){
    return yyparse();
    return 0;
}
