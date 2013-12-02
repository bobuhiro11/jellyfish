%{
/**********************************************
 * parser.y
 **********************************************/
#include "common.h"  
%}
/* type of grammar,token */
%union {
  int t_int;
  char t_char;
  char *t_symbol;
  struct s_exp *t_sexp;
}

/* token */
%token <t_int>    INTEGER 
%token <t_char>   CHAR
%token <t_symbol> SYMBOL
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
      | exp EOL       { printf ("%d\n", ($1->u).integer); prompt(); }
      ;
exp  :  INTEGER       { $$ = integer2sexp($1);}
      | LEFT_PAREN SYMBOL exp exp  RIGHT_PAREN
        { 
          $$ = (struct s_exp*)malloc(sizeof(struct s_exp));
          ($$->u).integer = ($3->u).integer + ($4->u).integer;
        }
      /*
      | '(' '-' exp exp ')' { $$ = $3 - $4; }
      | '(' '*' exp exp ')' { $$ = $3 * $4; }
      | '(' '/' exp exp ')' { $$ = $3 / $4; }
      */
      ;
%%
