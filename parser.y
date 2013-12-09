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
  char *t_special;
  struct s_exp *t_sexp;
}

/* token */
%token <t_int>      INTEGER 
%token <t_char>     CHARACTER
%token <t_symbol>   SYMBOL
%token <t_special>  SPECIAL
%token NIL
%token TRUE
%token FALSE
%token QUOTE
%token LEFT_PAREN
%token RIGHT_PAREN
%token EOL

/* type of grammar */
%type <t_sexp> exp
%type <t_sexp> members
%type <t_sexp> exp_noeval
%type <t_sexp> members_noeval

%%
input   :
        | input line    {}
        ;
line    : EOL           { prompt();  }
        | exp EOL       { write_sexp($1); putc('\n',stdout); prompt(); }
        ;
exp     : INTEGER       { $$ = integer2sexp($1);}
        | CHARACTER     { $$ = character2sexp($1);}
        | NIL           { $$ = nil;}
        | TRUE          { $$ = sexp_t;}
        | FALSE         { $$ = sexp_f;}
        | SYMBOL        
          { 
              struct s_exp *p = symbol2sexp($1);
              $$ = eval(p);
          }
        | QUOTE exp_noeval
          {
              $$ = $2;
          }
        | LEFT_PAREN SPECIAL members_noeval
          {
              struct s_exp *p = symbol2sexp($2);
              struct s_exp *e = cons(p,$3);
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
            $$ = cons($1,$2);
          }
        ;
exp_noeval : INTEGER    { $$ = integer2sexp($1);}     /* no evalute s-expression for special operator */
        | CHARACTER     { $$ = character2sexp($1);}
        | SYMBOL        { $$ = symbol2sexp($1);}
        | NIL           { $$ = nil;}
	      | TRUE		      { $$ = sexp_t;}
        | FALSE		      { $$ = sexp_f;}
        | QUOTE exp_noeval
          {
              $$ = $2;
          }
        | LEFT_PAREN members_noeval
          { 
               $$ = $2;
          }
        | LEFT_PAREN SPECIAL members_noeval
          {
              struct s_exp *p = symbol2sexp($2);
              struct s_exp *e = cons(p,$3);
              $$ = e;
          }
        ;
members_noeval : RIGHT_PAREN  { $$ = nil; }
        | exp_noeval members_noeval
          {
            $$ = cons($1,$2);
          }
        ;
%%
