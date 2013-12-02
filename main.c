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
