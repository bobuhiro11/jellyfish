%{
/**********************************************
 * lexer.l
 **********************************************/
#include "common.h"

static int paren_num=0;

%}

%%
";".*"\n"                           { }
[ \t]+                              { }
[-]?[0-9]+   			                  { sscanf(yytext, "%d", (int*)&yylval); return INTEGER; }
[(]                  		            { paren_num ++; return LEFT_PAREN; }
[)]         		                    {	paren_num --; return RIGHT_PAREN; }
"\n"                                { if(paren_num != 0) prompt_multi();}
"nil"                               { return NIL; }
"#t"	                              { return TRUE; }
"#f"	                              { return FALSE; }
"'"	                                { return QUOTE; }
[a-z|\+|\-|\*|\/|?|=|>|<]+[a-zA-Z0-9]*    {
  int n = strlen(yytext);
  char *p = (char*)malloc(n+1);
  yylval.t_symbol = p;
  strcpy(yylval.t_symbol,yytext);
  return SYMBOL;
}
%%
