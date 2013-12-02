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
      | exp EOL       { write_sexp($1); putc('\n',stdout); prompt(); }
      ;
exp   : INTEGER       { $$ = integer2sexp($1);}
      | LEFT_PAREN SYMBOL exp exp  RIGHT_PAREN
        { 
	 struct s_exp *p = symbol2sexp($2);
	 struct s_exp *e = pair2sexp(p, pair2sexp($3, pair2sexp($4,nil)));	
	 e = eval(e);
	 $$ = e;
        }
      ;
%%
