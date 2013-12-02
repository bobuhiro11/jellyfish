%{
/**********************************************
 * parser.y
 **********************************************/
#include "common.h"  
%}
/* type of grammar,token */
%union {
  int t_int;
  double t_double;
  char *t_string;
  struct s_exp *t_sexp;
}

/* token */
%token <t_int>    INTEGER 
%token <t_string> STRING
%token <t_string> SYMBOL
%token LEFT_PAREN
%token RIGHT_PAREN
%token EOL

/* type of grammar */
%type <t_sexp> exp

%%
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
