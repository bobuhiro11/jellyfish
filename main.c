#include "common.h"

/*
 * true when interactive mode, false otherwise
 */
int interactive;

/*
 *  invalid token, grammar
 */
void yyerror(char* s) {
	printf("Error: %s\n", s);
}

/*
 * show prompt before input
 */
void prompt(){ 
	if(interactive)
		printf("jellyfish> ");
}

int yywrap() { 
	return 1; 
}

/*
 * create s_exp from integer
 */
struct s_exp *integer2sexp(int i){
	struct s_exp *e =  (struct s_exp*)malloc(sizeof(struct s_exp));
	e->type = S_EXP_INTEGER;
	(e->u).integer = i;
	return e;
}

/*
 * create s_exp from character
 */
struct s_exp *character2sexp(char c){
	struct s_exp *e =  (struct s_exp*)malloc(sizeof(struct s_exp));
	e->type = S_EXP_CHARACTER;
	(e->u).character = c;
	return e;
}

int main(int argc, char **argv) {
	char *p;
	if(argc > 1){
		/* from source code */
		interactive = 0;
		if ((yyin = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr,"Error: can't open file \"%s\".\n",argv[1]);
			exit(EXIT_FAILURE);
		}
	}else{
		/* from interactive input */
		interactive = 1;
		yyin = stdin;
		prompt();
	}
	yyparse();
	return 0;
}
