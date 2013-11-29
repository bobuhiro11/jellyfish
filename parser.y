%{
 /* C宣言部 ─ 動作記述の中で用いる関数の定義や宣言 */
#define YYSTYPE double  /* トークンの属性の型を宣言 */
#include <stdio.h>
#include <stdlib.h>           /* exit関数を使うため */
extern FILE *yyin;  
int interactive;

int yylex(void); /* yylexのプロトタイプ宣言 */

void yyerror(char* s) {
   printf("Error: %s\n", s);
}

void prompt(){
  if(interactive)
    printf(">");
}

%}

%token NUMBER 
%token EOL
%left '+' '-'
%left '*' '/'
%%
input :   /* 空 */
      | input line    {}
      ;
line  : EOL           { prompt();  }
      | expr EOL      { printf ("%f\n", $1); prompt(); }
      ;
expr :  NUMBER        { $$ = $1; }
      | '(' '+' expr expr ')' { $$ = $3 + $4; }
      | '(' '-' expr expr ')' { $$ = $3 - $4; }
      | '(' '*' expr expr ')' { $$ = $3 * $4; }
      | '(' '/' expr expr ')' { $$ = $3 / $4; }
      ;
%%

int main(int argc, char **argv) {
  char *p;
  if(argc > 1){
    /* ソースコード読み込み */
    interactive = 0;
    if ((yyin = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr,"Error: can't open file \"%s\".\n",argv[1]);
      exit(EXIT_FAILURE);
    }
  }else{
    /* 対話方式 */
    interactive = 1;
    yyin = stdin;
    prompt();
  }
  yyparse();
  return 0;
}
