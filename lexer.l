%{
/**********************************************
 * lexer.l
 **********************************************/
#include "common.h"
%}

%%
[ \t]+                              {  }
[0-9]+(\.[0-9]+)?(E[+\-]?[0-9]+)?   { sscanf(yytext, "%d", (int*)&yylval); return INTEGER; }
[(]                  		            { return LEFT_PAREN; }
[)]         		                    {	return RIGHT_PAREN; }
"\n"                                { return EOL; }
"."                                 { return DOT; }
"nil"                               { return NIL; }
.                                   { 
  int n = strlen(yytext);
  char *p = (char*)malloc(n+1);
  yylval.t_symbol = p;
  strcpy(yylval.t_symbol,yytext);
  return SYMBOL;
}
%%
