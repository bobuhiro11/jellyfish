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
%token <t_char>   CHARACTER
%token <t_symbol> SYMBOL
%token NIL
%token DOT
%token LEFT_PAREN
%token RIGHT_PAREN
%token EOL

/* type of grammar */
%type <t_sexp> exp
%type <t_sexp> members

%%
input   :   
        | input line    {}
        ;
line    : EOL           { prompt();  }
        | exp EOL       { write_sexp($1); putc('\n',stdout); prompt(); }
        ;
exp     : INTEGER       { $$ = integer2sexp($1);}
        | CHARACTER     { $$ = character2sexp($1);}
        | SYMBOL        { $$ = symbol2sexp($1);}
        | NIL           { $$ = nil;}
        | LEFT_PAREN exp DOT exp RIGHT_PAREN
          {
              struct s_exp *e = pair2sexp($2,$4);
              $$ = eval(e);
          }
        | LEFT_PAREN members
          { 
             $$ = eval($2);
          }
        ;
members : RIGHT_PAREN  { $$ = nil; }
        | exp members  
          {
            $$ = pair2sexp($1,$2);
          }
        ;
%%
