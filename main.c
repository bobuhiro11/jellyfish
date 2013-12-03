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

/*
 * create s_exp from symbol
 */
struct s_exp *symbol2sexp(char *s){
	struct s_exp *e =  (struct s_exp*)malloc(sizeof(struct s_exp));
	e->type = S_EXP_SYMBOL;
	(e->u).symbol = s;
	return e;
}

/*
 * create s_exp from dotted pair
 */
struct s_exp *cons(struct s_exp *car, struct s_exp *cdr){
	struct s_exp *e =  (struct s_exp*)malloc(sizeof(struct s_exp));
	e->type = S_EXP_PAIR;
	(e->u).pair.car = car;
	(e->u).pair.cdr = cdr;
	return e;
}

static void _write_type(struct s_exp *e){
	if(e == nil)
		printf("");
	else if(e->type == S_EXP_INTEGER)
		printf("<INTEGER>");
	else if(e->type == S_EXP_CHARACTER)
		printf("<CHARACTER>");
	else if(e->type == S_EXP_SYMBOL)
		printf("<SYMBOL>");
	else if(e->type == S_EXP_PAIR)
		printf("<PAIR>");
}

static void _write_sexp(struct s_exp *e, int d){
	if(d==0)
		_write_type(e);

	if(e == nil){
		printf("nil");
	}else if(e->type == S_EXP_INTEGER){
		printf("%d",e->u.integer);
	}else if(e->type == S_EXP_CHARACTER){
		printf("%c",e->u.character);
	}else if(e->type == S_EXP_SYMBOL){
		printf("%s",e->u.symbol);
	}else{
		printf("(");
		_write_sexp(e->u.pair.car,d+1);
		printf(" . ");
		_write_sexp(e->u.pair.cdr,d+1);
		printf(")");
	}
}

void write_sexp(struct s_exp *e){
	_write_sexp(e,0);
}

/*
 * add function
 */
static struct s_exp *add(struct s_exp *args){
	struct s_exp *p = args;
	struct s_exp *q,*t;
	int s = 0;
	while(p != nil){
		q = eval(p->u.pair.car);
		s += q->u.integer;
		t = p->u.pair.cdr;
		free(p);
		p = t;
	}
	return integer2sexp(s);
}

/*
 * eval s expression
 *
 * 	(1) atom
 * 	(2) special operation
 * 	(3) function
 *
 */
struct s_exp *eval(struct s_exp *e){
	if(e == nil){					/* (1) nil */
		return nil;
	}else if(e->type == S_EXP_INTEGER){		/* (1) integer */
		return e;
	}else if(e->type == S_EXP_CHARACTER){		/* (1) character */
		return e;
	}else if(e->type == S_EXP_SYMBOL){		/* (1) symbol */
		/* fix */
		return e;
	}else{
		/* fix http://melborne.github.io/2010/11/10/Ruby-Lisp/ */
		struct s_exp *car = e->u.pair.car;
		struct s_exp *cdr = e->u.pair.cdr;
		if(!strcmp(car->u.symbol, "quote")){	/* (2) (quote exp) */
			return cdr;
		}
		else{ 					/* (3) function apply */
			free(e);
			return apply(car,cdr);
		}
	}
}

/*
 * apply basic function
 */
struct s_exp *apply(struct s_exp *func, struct s_exp *args){
	char *f_name = func->u.symbol;

	if(!strcmp(f_name,"+")){
		return add(args);
	}
	return nil;
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
