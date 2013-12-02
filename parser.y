%{
/*
 * parser.y
 */
#define YYSTYPE struct s_exp*      /* type of each grammar */
#include <stdio.h>
#include <stdlib.h>                /* for exit() */

#define S_EXP_ATOM 1               /* type of s_exp */
#define S_EXP_PAIR 2

#define ATOM_INTEGER 1             /* type of atom */
struct atom{                       /* atom */
  int type;                        /* ATOM_INTEGER */
  union{
    int _int;
  }u;
};

struct pair{                       /* dotted pair */
  struct s_exp *car;
  struct s_exp *cdr;
};

struct s_exp{                      /* s exception */
  int type;                        /* S_EXP_ATOM or S_EXP_PAIR */
  union{
    struct atom *atom;
    struct pair *pair;
  }u;
} s_exp;
  
extern char *yytext;               /* lexer input */
extern FILE *yyin;                 /* input file */
int interactive;                   /* true if interactive mode, false otherwise */

int yylex(void);

void yyerror(char* s) {
   printf("Error: %s\n", s);
}

/*
 * show prompt before input
 */
void prompt(){ 
  if(interactive)
    printf(">");
}

%}

/*
 * token
 */
%token INTEGER 
%token STRING
%token SYMBOL
%token LEFT_PAREN
%token RIGHT_PAREN
%token EOL
/*
 * association
 */
/*
%left '+' '-'
%left '*' '/'
*/
%%
/*
 * Grammar
 */
input :   
      | input line    {}
      ;
line  : EOL           { prompt();  }
      | exp EOL       { printf ("%d\n", ($1->u.atom)->u._int); prompt(); }
      ;
exp  :  INTEGER        
        { 
          $$ = (struct s_exp*)malloc(sizeof(struct s_exp));
          struct atom *a = malloc(sizeof(struct atom));
          ($$->u).atom = a;
          (a->u)._int = (int)$1;
        }
      | '(' '+' exp exp ')' 
        { 
          $$ = (struct s_exp*)malloc(sizeof(struct s_exp));
          struct atom *a = malloc(sizeof(struct atom));
          ($$->u).atom = a;
          (a->u)._int = ($3->u.atom)->u._int + ($4->u.atom)->u._int;
        }
      /*
      | '(' '-' exp exp ')' { $$ = $3 - $4; }
      | '(' '*' exp exp ')' { $$ = $3 * $4; }
      | '(' '/' exp exp ')' { $$ = $3 / $4; }
      */
      ;
%%

int main(int argc, char **argv) {
  char *p;
  if(argc > 1){
    /* ソースコード読み込み */
    interactive = 0;
    if ((yyin = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr,"Error: can't open file \"%s\".\n",argv[1]);
      exit(EXIT_FAILURE);
    }
  }else{
    /* 対話方式 */
    interactive = 1;
    yyin = stdin;
    prompt();
  }
  yyparse();
  return 0;
}
