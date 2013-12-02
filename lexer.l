%{
/**********************************************
 * lexer.l
 **********************************************/
#include "common.h"
%}

%%
[ \t]+                              {  }
[0-9]+(\.[0-9]+)?(E[+\-]?[0-9]+)?   { 
                                      sscanf(yytext, "%d", (int*)&yylval);
				      return INTEGER; 
                                    }
[+\-\*\/\=]  	                    {
                                      return yytext[0];
                                    }
[(]         		            {
                                      return LEFT_PAREN;
                                    }
[)]         		            {
                                      return RIGHT_PAREN;
                                    }
"\n"                                { return EOL; }
.                                   { yyerror("invalid token."); return EOL; } 
%%
