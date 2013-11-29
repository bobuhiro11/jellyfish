%{
void yyerror(char*);

#define YY_SKIP_YYWRAP
#define YYSTYPE double  /* トークンの属性の型 */
int yywrap() { return 1; }
#include "parser.h"     /* parser.hは bisonが生成するファイルトークン名が含まれる */
%}
%%
[ \t]+                              {  }
[0-9]+(\.[0-9]+)?(E[+\-]?[0-9]+)?   { 
                                      sscanf(yytext, "%lf", &yylval); return NUMBER; 
                                    }
[+\-\*\/\(\)\=]                     { /* + - * / ( ) =の場合は、マッチした文字をそのまま返す。*/
                                      /* マッチした文字は一般に yytext[0] ～ yytext[yyleng-1]。*/
                                      return yytext[0];
                                    }
"\n"                                { return EOL; }
.                                   { yyerror("Error: invalid token."); return EOL; } 
%%
