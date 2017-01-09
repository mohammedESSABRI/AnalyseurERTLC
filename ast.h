

//ce fichier contient juste les prototypes des fonctions, les variables globales ainsi que
//la structure ast
//les définitions des fonctions sont dans ast.c

#define m 300

char* tab_remplir[m];//les elements de l'ast
int i;//pour se déplacer dans tab_remplir[]
typedef struct ast
{
enum { CONCAT, UNION, ETOILE, A, B, EPSILON, VIDE } type;
struct ast *fg; //noeud gauche
struct ast *fd; //noeud droit 
}ast;


//la création des terminaux
struct ast* creer_a();
struct ast* creer_b();
struct ast* creer_epsilon();
struct ast* creer_vide();

// la création des operateurs
struct ast* creer_union(struct ast *e1, struct ast *e2);
struct ast* creer_concat(struct ast *e1, struct ast *e2);
struct ast* creer_etoile(struct ast *e);

//manupilations supplémentaires de l'ast
struct ast* copier_ast(struct ast *e);

void liberer_ast(struct ast *e);

//stockage de l'ast dans le tableau tab remplir
void remplir_elements_ast(struct ast *e);

//stockage de l'ast dans un fichier xml
void imprimer_ast(struct ast *e);

//retourne 1 si epsilon appartient au langage de e
int contient_epsilon(struct ast *e);

//retourne le résiduel de e par rapport au caractère s
struct ast* residuel(char s, struct ast *e);

//retourne 1 sie accepte e
int appartient(char *w, struct ast *e);

//retourne 1 si e n'accepte aucune mot
int est_vide(struct ast* e);

//retourne 1 si e n'accepte que epsilon
int est_epsilon(struct ast* e);

//retourner 1 si le langage de e ={a} ou {b}
int est_symbole(struct ast* e);

//returne 1 si l(e) est formé d'un seul mot
int est_single(struct ast *e);

//retourner 1 si e accepte un nombre fini de mots
int est_fini(struct ast *e);




//// supplementaire, pour l'interface utilisateur




     
// les informations de l'utilisateur

typedef struct {
        int jrr;
        int mss;
        int ann;
        
        }dtt;
typedef struct USR{
        char nom[30];
        char prenom[30];
        dtt nss;
        char pass[20];
        }usr;
        
/*        
void err(const char* a, int b);

void  lcmd();
void tcmd();
int ysn();
void bd();
void printusr(usr a);
void qt();
void txt(int k);
void hlp(int k);
void hlpcmd(const char* hlpa);

void clss();
void regr();
void acc(); 


*/
