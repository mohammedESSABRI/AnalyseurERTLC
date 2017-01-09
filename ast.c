
#include<stdio.h>
#include<stdlib.h> 
#include <string.h>
#include "ast.h"




struct ast* creer_a(){
	ast* as=(ast*)malloc(1*sizeof(ast));
	as->type=A;
	as->fg=NULL;
	as->fd=NULL;
	return as;
}



struct ast* creer_b(){
	ast* as=(ast*)malloc(1*sizeof(ast));
	as->type=B;
	as->fg=NULL;
	as->fd=NULL;
	return as;
}



struct ast* creer_epsilon(){
    ast* as=(ast*)malloc(1*sizeof(ast));
    as->type=EPSILON;
	as->fg=NULL;
	as->fd=NULL;
	return as;
}


struct ast* creer_vide(){
	ast* as=(ast*)malloc(1*sizeof(ast));
    as->type=VIDE ;
	as->fg=NULL;
	as->fd=NULL;
	return as;
	
}



struct ast* creer_union(struct ast *e1, struct ast *e2){
    ast* as=(ast*)malloc(sizeof(ast));
	as->type=UNION ;
	as->fg=e1;
	as->fd=e2;
	return as;
	
	
}
struct ast* creer_concat(struct ast *e1, struct ast *e2){
    ast* as=(ast*)malloc(sizeof(ast));
	as->type=CONCAT ;
	as->fg=e1;
	as->fd=e2;
	return as;
	

	
}



struct ast* creer_etoile(struct ast *e){
	ast* as=(ast*)malloc(sizeof(ast));
	as->type=ETOILE ;
	as->fg=e;
	as->fd=NULL;
	return as;
	
}



struct ast* copier_ast(struct ast *e){
    ast* as=(ast*)malloc(sizeof(ast));
	as->type=e->type;
	as->fg=e->fg;
	as->fd=e->fd;
	return as;
	
	
}



void liberer_ast(struct ast *e){
   if(e!=NULL){ //dans le cas contraire, les testes ne sont pas demandés, puisque e sera déja libéré
    if((e->fg)!=NULL){
                      liberer_ast(e->fg);
                      if((e->fd)!=NULL)liberer_ast(e->fd);
                      
                      }
                free(e);
                e=NULL;
                i=0;//initialiser l'indice du tableau tab_remplir
                }  //à fin de l'utiliser à nouveau
}




void remplir_elements_ast(struct ast *e){
     	switch(e->type){
                        
case A : { tab_remplir[i]= malloc(5*sizeof(char));//5 car lengthof(<a/>)=4+1 du caractère de fin
           strcpy(tab_remplir[i],"<a/>") ;
           i++;
           }break;
           
case B : { tab_remplir[i]=malloc(5*sizeof(char));
                   strcpy(tab_remplir[i],"<b/>") ;
                   i++;
                   } break;
                   
                   
case EPSILON : {tab_remplir[i]=malloc(11*sizeof(char));
                   strcpy(tab_remplir[i],"<epsilon/>") ;
                   i++;
                   }break; 
                   
                   
case VIDE: {tab_remplir[i]=malloc(8*sizeof(char));
                   strcpy(tab_remplir[i],"<vide/>");
                   i++;  
                   }break;       
                   
case ETOILE : {tab_remplir[i]=malloc(9*sizeof(char));
                       strcpy(tab_remplir[i],"<etoile>") ;
                       i++;
                       remplir_elements_ast(e->fg);
                       tab_remplir[i]=malloc(10*sizeof(char));
                       strcpy(tab_remplir[i],"</etoile>") ;
                   i++;
                   }break; 
                     
case UNION : {tab_remplir[i]=malloc(8*sizeof(char));
                   strcpy(tab_remplir[i],"<union>") ; 
                   i++;
                   remplir_elements_ast(e->fg);
                   remplir_elements_ast(e->fd);
                   tab_remplir[i]=malloc(9*sizeof(char));
                   strcpy(tab_remplir[i],"</union>") ; 
                   i++;
                   }break;

case CONCAT:  {tab_remplir[i]=malloc(9*sizeof(char));
                   strcpy(tab_remplir[i],"<concat>") ;
                   i++;
                   remplir_elements_ast(e->fg);
                   remplir_elements_ast(e->fd); 
                   tab_remplir[i]=malloc(10*sizeof(char));
                   strcpy(tab_remplir[i],"</concat>");
                   i++;
                   }
                    }            
                                                  
     }
     
     
void imprimer_ast(struct ast *e){
     FILE *fp=fopen("ast.xml","w");//le mode w pour supprimer l'ancien ast s'il exite un
     int k;
     
     fprintf(fp ,"%s","<ast>");
     puts("<ast>");
     remplir_elements_ast(e);
     for(k=0;k<i;k++){
     fprintf(fp,"%s",tab_remplir[k]);
     puts(tab_remplir[k]);}
     fprintf(fp ,"%s","</ast>");
     puts("</ast>");
     fclose(fp);
     }
     
     
     
int contient_epsilon(struct ast *e){
    switch(e->type){
         case A :
		case B : 
		case VIDE : return 0;
		case EPSILON: 
        case ETOILE : return 1;
        //une concatination est un epsilon si ses operants le sont tous
        case CONCAT : return  contient_epsilon(e->fg) && contient_epsilon(e->fd);
        //une union est un epsilon si ses operants le sont tous
        case UNION  : return  contient_epsilon(e->fg)|| contient_epsilon(e->fd);
        }
}


struct ast* residuel(char s, struct ast *e)
{  
       switch(e->type){
                   case VIDE : 
                   case EPSILON : return creer_vide();
                   
                   case A : {
                        
                        if(s=='a')return creer_epsilon(); //x/x=epsilo,
                        if(s=='1')return creer_a(); //x/epsilon reste lui même
                        return  creer_vide();//ce cas correspond à x/y avec x!=y
                           }
                           
                   case B : {
                           if(s=='b')return creer_epsilon(); 
                           if(s=='1')return creer_b();
                           return creer_vide();
                           }
                           
                   case CONCAT : {
                        
                        if(contient_epsilon(e->fg))
                        //si epsilon est dans L(r),(rs)/x=(r/s)s|s/x
                        return  creer_union(creer_concat(residuel(s, e->fg),e->fd),residuel(s, e->fd));
                        else //si epsilon n'est pas dans L(r),(rs)/x=(r/x)s
                        return creer_concat(residuel(s, e->fg),e->fd);
                                 
                                 }
                        //(r|s)/x=r/x|s/x
                   case UNION  : return   creer_union(residuel(s, e->fg),residuel(s, e->fd));
                        //r*/s=(r/s)r*    
                    case ETOILE : return creer_concat(residuel(s, e->fg),creer_etoile(e->fg));  
                          }
       }
       
       
       
int est_vide(struct ast* e){
    
     switch(e->type){
        case A :
		case B : 
		case EPSILON: return 0;
		case VIDE :  return 1; 
        case ETOILE : return 0;//le plus petit ensemble c'est le vide el l'étoile
                               // de celui çi contient epsilon
            
        case CONCAT : return  est_vide(e->fg)|| est_vide(e->fd);
        case UNION  : return  est_vide(e->fg)&& est_vide(e->fd);
        }
}  
int est_epsilon(struct ast* e){ 
 switch(e->type){
        case A :
		case B : 
		case VIDE : return 0;
		case EPSILON: return 1; 
        case ETOILE : {
              if (e->fg->type==VIDE)return 1; //vide*=epsilon
              return  est_epsilon(e->fg);//epsilon*=epsilon, les autres non
              }
        case CONCAT : return  est_epsilon(e->fg)&& est_epsilon(e->fd);
        case UNION  : return  est_epsilon(e->fg)|| est_epsilon(e->fd);
        }
} 


int est_symbole(struct ast* e){
 switch(e->type){
        case A :
		case B : return 1;
		case VIDE : 
		case EPSILON: return 0; 
        case ETOILE : return  0;
        case CONCAT : return ( est_symbole(e->fg)&& est_epsilon(e->fd))//l'un des deux membres est
                      || (est_symbole(e->fd)&& est_epsilon(e->fg));//un epsilon, l'autre est un
                                                                 //symbole a ou b
        case UNION  : {if(e->fg->type==e->fd->type)//si les deux sont les même, il suffit juste
                       return est_symbole(e->fg);//que l'un soit un symbole
             return  est_symbole(e->fg)&& est_vide(e->fd)//si non , un membre doit être symbole
             || est_symbole(e->fd)&& est_vide(e->fg);//l'autre doit être epsilon
             }
        }
}   


int est_single(struct ast* e){
    
    switch(e->type){
        case A :
       	case EPSILON:     
		case B : return 1;
		case VIDE :return 0;
        case ETOILE : return est_vide(e->fg);
        case CONCAT : return ((est_single(e->fg)&& est_epsilon(e->fd))//il faut qu'un membre soit
           ||(est_epsilon(e->fg)&& est_single(e->fd))//epsilon et l'autre single
           ||(est_single(e->fd) && est_single(e->fg)));//ou touts les deux soient singles
       
                  // il faut que l'un des membre soit single et l'autre soit vide 
        case UNION : return  (est_single(e->fg)&& est_vide(e->fd))||(est_vide(e->fg)&& est_single(e->fd));
                
                   }         } 
                   
                   
                   
                   
                   
int appartient(char *s ,struct ast * e){
    
    if(e!=NULL){
    struct ast *a =e;
    while(*s){
    a=residuel(*s,a);
    s++;
    }
    return contient_epsilon(a);
     }
     }
     
     
     
     
                   
int est_fini(struct ast *e)
{ 
    switch(e->type){
        case A :
        case EPSILON:
		case B : 
		case VIDE :return 1; 
        case ETOILE : return   est_vide(e->fg)|| est_epsilon(e->fg);//epsilon est fini
        case CONCAT : 
        case UNION  : return  est_fini (e->fg)&& est_fini(e->fd);
        }
    } 
    
















//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//supplemntaire pour l'interface;
#define n 4 //la liste des commande
char cmnd[100];//prendre les commandes qui sont passées en langage c
int cnt=0;//=1 si un usr est connecté
int errr=0;//=1 lorsque il y a une erreur de commande
char* aide[n]={"conn","inscr","anonyme","intrp"};
int dcc=0; // pour la décision de l'affichage 

int scm(char* s,const char * c);
void scp(char* s,const char * c);
void clss();



// retourner 1 si deux chaines sont les mêmes

int scm(char* s,const char * c){
    if(strcmp(s,c)==0)return 1;
    else return 0;
    
    }

    
// copier deux chaines rapidement
    
void scp(char* s,const char * c){
     strcpy(s,c);
     
     }
//vide l'écran rapidement

   void clss(){
        system("cls");
        
        }


     

usr usrr;
void err(const char* a, int b){
     switch(b){
     case 1 : {printf("\nla commande \"%s\" est incorrecte",a);//cmd
              errr=0;}break;
     case 2 : {printf("\nerreur: %s",a);}break; //general
     
     case 3 : {
            printf("\nerreur: l'utilisateur %s %s n\'est pas trouve, verifie votre nom",usrr.nom,usrr.prenom);//usr nt found
          }
     }
     }




//l'affichage du nom de usr

void  lcmd(){
    if(cnt==0)printf("\n\nAnonyme >> ");
    else printf("\n \n%s : ",usrr.nom);
     }
     
//taper une commande     
void tcmd(){
          gets(cmnd);
          }
   
 //gerrer les questions binaires    
int ysn(){
    while(1){
     printf(" [yes/no] ?");
     lcmd();
     
     tcmd();
     if(scm(cmnd,"yes"))return 1;
     if(scm(cmnd,"no"))return 0;
     else err(cmnd,1);
     
     
     }
     }
     
//retourne l'emplacement du fichier à traiter c
void bd(){
     
     
     strcpy(cmnd,"data\\");
     strcat(cmnd,usrr.nom);
     strcat(cmnd," ");
     strcat(cmnd,usrr.prenom);
     strcat(cmnd,".bd");
     
    
     }

//affiche les informations 
void printusr(usr a){
     printf("\nnom:%s",a.nom);
     
     printf("\nprenom:%s",a.prenom);
     
     
     printf("\npass:%s",a.pass);
     
     printf("\nnee le:%d-%d-%d",a.nss.jrr,a.nss.mss,a.nss.ann);
     
     
     }

  

//quitter
void qt(){
     
     exit(0);
     }

// prend un indice et et affecte l'emplacement de l'aide pour la commande correspondante à la variable cmnd    

void txt(int k){
      
      scp(cmnd,"data\\hlp\\");
      strcat(cmnd,aide[k]);
      strcat(cmnd,".txt");
      
      }   
    

     
//barre de l'aide dans bison
void brrr(){
     system("pause");
     clss();
     printf("\n*********************************************");
     printf("\n*                                           *");
     printf("\n*   hlp:aide  qt:quitter  dnc:deconnecter   *");
     printf("\n*                                           *");
     printf("\n*********************************************\n");
     lcmd();
     }     
  
     
    
  
   
  
   
// aide et support

void hlp(int k){
    
     FILE* fp;
     int c;
     
     
     
     
    
     
     while(scm(cmnd,"eend")==0){
     clss();
  
  poss : ;
     printf("\n*************** AIDE ET SUPPORT *************************");
     printf("\n*                                                       *");        
     printf("\n* pres     : page precedente                            *");
     printf("\n* suiv     : page suivante                              *");
     printf("\n* commande : aller à au support concernant une commande *");
     printf("\n* eend     : fermer le menu de l\'aide                   *"); 
     printf("\n*                                                       *");
     printf("\n*********************************************************\n\n");
     
     txt(k);
     int llm;
     fp=fopen(cmnd,"r");
      llm='0';
      if(fp){
      while(llm!=EOF){
      llm=fgetc(fp);
      putchar(llm);
      }
      close(fp);
      }
     else {
          err("lors de chargement de la commande",2);
          if(ysn()==0)break;
          }
    
      
     lcmd();
     tcmd();
     
     if(scm(cmnd,"pres")){
                          
     if(k==0){
              err("vous etes sur la premiere page",2);
              if(ysn()==0)break;
             }  
     else { 
               k--;
               
         }} 
     else{    
     if(scm(cmnd,"suiv")){
     if(k==n-1) {err("vous etes sur la derniere page",2);
                if(ysn()==0)break;
                }
     else{
       k++;     
          }        
     
        }
             
     
     else {
          if(scm(cmnd,"fin"))break;
          else{if(scm(cmnd,"\n"))goto poss;
          err(cmnd,1);
     printf("\nvoulez-vous sortir du menu de l\'aide");
     if(ysn()==1)break;
     }
     }}
     }
     
     
     }


// aide associé à une commande

void hlpcmd(const char* hlpa){
     printf("hlpcmd:%s",hlpa);
    int kll,inc=0;
    
    for(kll=0;kll<n;kll++){
     printf("%d",kll);
    
     if(scm(aide[kll],hlpa)){inc=1;break;}
     
     }
     if(inc==1) hlp(kll);
     else err(hlpa,1);
     
     }
     
     
     
     
     
     
     
     
// menu de l'interface principale d'analyseur
     
void menu(){
     printf("menu");
     
     
     }
     

//connexion d'un usr
   
conct(){
    FILE* fp;
    poss : ;
    int icn;//taper les valeur entiers
    char* wrning="informations incorrectes";
    clss();
    printf("\n************************* CONNEXION ***************************");
    
    printf("\nsi vous avez oublie le mot de pass, taper la commande psss a la\nplace du nom,\n");
    printf("\nvoulez-vous continuer la connexion");
    if(ysn()==0)return 0;
    pssss : ;
    clss();
    printf("\n************************* CONNEXION ***************************");
    printf("\nNOM:");
    gets(usrr.nom);

if(scm(usrr.nom,"psss")){
                         poss2 : ;
                            clss(); 
                             printf("\nrecuperation du mot de passe\ntapez vos informations de connexion !\nnom:");
                             
                             gets(usrr.nom);
                             
                             printf("prenom:");
                             gets(usrr.prenom);
                             bd();
                             
                             fp=fopen(cmnd,"rb");
       if(fp){
                    if(fread(&usrr,sizeof(usr),1,fp)){//verifie la questionde sécurité
                                                      
                             cnt=1;
                             puts("\nquel est votre jour de naissance?");
                             lcmd();
                             scanf("%d",&icn);
                       if((icn)==usrr.nss.jrr)
                       {
                              puts("\nquel est votre mois de naissance?");
                              lcmd();
                              scanf("%d",&icn);
                                               if((icn)==usrr.nss.mss){
                                               
                                               puts("\nquel est votre annee de naissance?"); 
                                               lcmd();                       
                                               scanf("%d",&icn);
                                                 if((icn)==usrr.nss.ann)
                                                   {cnt=0;
                                                    printf("\nsucces, votre mot de passe est:%s\n",usrr.pass);
                                                    gets(cmnd);
                                                    printf("\nvoulez-vous continuer");
                                                    
                                                    if(ysn()) goto poss;
                                                    else exit(0);
                                                                                 
                                                    }
                                                 else{err(wrning,2);//réponse fausse
                                                      cnt=0;
                                                      }
                                                 
                                                                       }
                                               else {err(wrning,2);//réponse fausse
                                               cnt=0;}
                                               
                                              
                       }
                       
                      else {err(wrning,2);cnt=0;}
                                                    
                                                        }
                     else err("lors de lecture de donne sur la base de donnee",2);
            }
       else {err("m",3);
       printf("\nvoulez-vous continuer");
       if(ysn())goto poss;
       else ;
            }                 
 }
 else{//pas de récuperation de passe, continuer la connexion
    
    
    printf("\nPRENOM:");
    gets(usrr.prenom);
    
    bd();
    fp=fopen(cmnd,"rb");
    if(fp){
    if(fread(&usrr,sizeof(usr),1,fp)){       
    printf("\nmot de pass:");
    tcmd();
    
    if(scm(usrr.pass,cmnd)){printf("\nvous etes connecte en tant que %s %s",usrr.nom,usrr.prenom);
                            cnt=1;
                            printf("\nvoulez-vous continuer");
                            if(ysn())cnt=1;
                            else cnt=0;
                            }
    else{//erreur de mot passe
    
    err("mot de passe incorrecte",2);
    printf("\nvoulez-vous recuperer le mot de passe");
    if(ysn())goto poss2;
    else goto pssss;;
    }
}
    }
    else { err("",3);
         printf("\nvoulez-vous continuer");
         if(ysn())goto pssss;
         else cnt=0;
         }
}   
}
 



     
     
     
     
//inscription d'un nouveau usr 
void regr(){
     clss();
     printf("\n************************ INSCRIPTION ************************");
    
    FILE* fp;
    ps1 : ; 
    printf("\nNOM:");
    gets(usrr.nom);
    printf("\nPRENOM:");
    gets(usrr.prenom);
    
    
    bd();
   
    fp=fopen(cmnd,"rb");
    if(fp){
           err("utilisateur existe deja!!\n",2);
           printf("\nvoulez-vous continue");
           if(ysn())goto ps1;
           else return;
           fclose(fp);
           }
    
    printf("\nnaissance:\n\n>>JOUR:");
    scanf("%d",&usrr.nss.jrr);
    
    printf("\n>>MOIS:");
    scanf("%d",&usrr.nss.mss);
    printf("\n>>ANNEE:");
    scanf("%d",&usrr.nss.ann);
    gets(cmnd);
    ps2 : ;
    
    printf("\nmot de passe: ");
    
    gets(usrr.pass);
    printf("\nsaisir le mot de passe une deuxieme fois: ");
    tcmd();
    if(scm(cmnd,usrr.pass)==0){
                               err("mots de passes ne sont pas identiques",2);
                               goto ps2;
     
                              }
                              
                              
                              
                            
     //vider l'écrant
     clss();
     printusr(usrr);
     printf("\nvoulez-vous enregistrer ces informations");
     if(ysn()){
               
               bd();
               fp=fopen(cmnd,"wb");
               if(fp){
                      
                      if(fwrite(&usrr,sizeof(usr),1,fp)){clss();
                      printf("message:\n\nfelecitation %s,\nvous pouvez connecter et uliliser l\'analyseur\n",usrr.prenom);
                      printf("voulez vous contiuer");
                      if(ysn()==0)exit(0);                      
                                                         }
                      fclose(fp);
                      
                      bd();
                      fp=fopen(cmnd,"rb");
                     
                      }
               else err("impossible de creer le fichier stockant la bd",2);
               }
    }
   
   
   

     
     
     //menu d'acceuil
void acc(){
     
     /*printf("\n    **************************BIENVENU****************************");
     printf("\n    *                                                            *");
     printf("\n    *  [conn]           [inscr]              [anonyme]           *");
     printf("\n    * connecter   nouveau utilisateur   lancer sans inscriptions *");
     printf("\n    *                                                            *");
     printf("\n    **************************************************************");
     */
     poss : ;
     clss();
     printf("\n******************* BIENVENU *******************");
     printf("\n*                                              *      <<MINI_PROJET>>");
     printf("\n* conn              : connecter                *     Interpreteur des ER");
     printf("\n* inscr             : nouveau utilisateur      * Pour Monsieur le Professeur ");
     printf("\n* anonyme           : lancer sans inscriptions *     Abdelmajid Dargham");
     printf("\n* hlp | hlp commande: obtenir de l'aide        *    par Mohammed Essabri");
     printf("\n* qt                : quitter                  *        CNE:1129878449"); 
     printf("\n*                                              *     evalue le: 10-02-2015");
     printf("\n************************************************");
     if(errr==1)err(cmnd,1);
     
    
     while(1){
     lcmd();
     tcmd();
     if(scm(cmnd,"conn")){
                          conct();
                          if(cnt==0)goto poss;
                          else break;
                          }
     else{
          
     if(scm(cmnd,"inscr")){
                           regr();
                           goto poss;
                           }
     else {
     if(scm(cmnd,"anonyme")){
     clss();
     printf("\nattention !\nvous allez perdre votre historique une fois le programme est quitte\nvous pouvez obtenir de l'aide en tapant la commande \"help hist\" !!");
     printf("\nvoulez-vous continuer?");
     
     if(ysn()){
               
               cnt=0;
               break;
               }
     
     else {
          system("cls");
        
          }                             
     }
     else {
     if(scm(cmnd,"qt")){
                        printf("\nvolez-vous vraiment quitter");
                        if(ysn())
                        qt();
                        else goto poss;
                        
                        }
     else {
          
     if(scm(cmnd,"hlp")){
                         hlp(0);
                         goto poss;
                         }
     
     else {
          if((cmnd[3]=' ')&&cmnd[4]!='\0'){
                        
          char* pccc=cmnd+4;//pour permet d'obtenir de l'aide pr une cmd
          hlpcmd(pccc);
          goto poss;
          
          
          }
          else
          {
          errr=1;
          goto poss;
          }
          }
     
     }}}}
     
     }
     }
     void dnc(){
     cnt=0;
     acc();
     }
     
   
   

     
    
    
