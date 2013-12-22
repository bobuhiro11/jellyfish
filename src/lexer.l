%{
/**********************************************
 * lexer.l
 **********************************************/
#include "../include/common.h"

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
"'"	                                { 
	int n = strlen("quote");
	char *p = (char*)malloc(n+1);
	yylval.t_symbol = p;
	strcpy(yylval.t_symbol,"quote");
	return QUOTE;
}
[a-z|A-Z|\+|\-|\*|\/|?|=|>|<|_]+[a-zA-Z0-9]*    {
  int n = strlen(yytext);
  char *p = (char*)malloc(n+1);
  yylval.t_symbol = p;
  strcpy(yylval.t_symbol,yytext);
  return SYMBOL;
}
\"(\\.|[^"])*\"                           {
  int n = strlen(yytext);
  char *p = (char*)malloc(n+1);
  memset(p, 0, n+1);
  yylval.t_symbol = p;
  strncpy(yylval.t_string, yytext+1, n-2);
  return STRING;
}
%%
