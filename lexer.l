%{
/**********************************************
 * lexer.l
 **********************************************/
#include "common.h"
%}

%%
[ \t]+                              {  }
[-]?[0-9]+   			    { sscanf(yytext, "%d", (int*)&yylval); return INTEGER; }
[(]                  		            { return LEFT_PAREN; }
[)]         		                    {	return RIGHT_PAREN; }
"\n"                                { return EOL; }
"nil"                               { return NIL; }
"quote"                             { 
  int n = strlen(yytext);
  char *p = (char*)malloc(n+1);
  yylval.t_symbol = p;
  strcpy(yylval.t_symbol,yytext);
  return SPECIAL; 
 }
[a-z|\+|\-|\*|\/]+                  { 
  int n = strlen(yytext);
  char *p = (char*)malloc(n+1);
  yylval.t_symbol = p;
  strcpy(yylval.t_symbol,yytext);
  return SYMBOL;
}
%%
