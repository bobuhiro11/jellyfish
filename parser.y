%{
/**********************************************
 * parser.y
 **********************************************/
#include "common.h"  
%}
/* type of grammar,token */
%union {
  int  t_int;
  char t_char;
  char *t_symbol;
  char *t_string;
  struct s_exp *t_sexp;
}

/* token */
%token <t_int>      INTEGER 
%token <t_char>     CHARACTER
%token <t_symbol>   SYMBOL
%token <t_string>   STRING
%token NIL
%token TRUE
%token FALSE
%token QUOTE
%token LEFT_PAREN
%token RIGHT_PAREN

/* type of grammar */
%type <t_sexp> exp_noeval
%type <t_sexp> members_noeval

%%
input   :
        | input exp_noeval    
          {
            struct s_exp *e = eval($2); 
            if(interactive){
              write_sexp(e); 
              putc('\n',stdout); 
              prompt(); 
            }
          }
exp_noeval : INTEGER    { $$ = integer2sexp($1);}     /* no evalute s-expression for special operator */
        | CHARACTER     { $$ = character2sexp($1);}
        | SYMBOL        { $$ = symbol2sexp($1);}
        | STRING        { $$ = string2sexp($1);}
        | NIL           { $$ = nil;}
	      | TRUE		      { $$ = sexp_t;}
        | FALSE		      { $$ = sexp_f;}
        | QUOTE exp_noeval
          {
               struct s_exp *car = symbol2sexp("quote");
               struct s_exp *cdr = cons($2, nil);
               $$ = cons(car,cdr);
          }
        | LEFT_PAREN members_noeval
          { 
               $$ = $2;
          }
        ;
members_noeval : RIGHT_PAREN  { $$ = nil; }
        | exp_noeval members_noeval
          {
            $$ = cons($1,$2);
          }
        ;
%%
