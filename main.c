#include "common.h"

/*
 * true when interactive mode, false otherwise
 */
int interactive;

/*
 * line number incremented by parser
 */
int linenum=0;

/*
 * global symbol table.
 */
struct symbol_table *global_table;

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
		printf("[%d]> ",linenum++);
}

int yywrap() { 
	return 1; 
}

/*
 * alloc new sexp object.
 * return sexp object if success, NULL otherwise.
 */
struct s_exp *sexp_alloc(){
	struct s_exp *e =  (struct s_exp*)malloc(sizeof(struct s_exp));
	if(!e)
		return NULL;
	e->ref = 1;
	return e;
}

/*
 * ref new sexp object.
 * return sexp object if success, NULL otherwise.
 */
struct s_exp *sexp_ref(struct s_exp *e){
	e->ref++;
	return e;
}

/*
 * free sexp object if reference count is 0.
 */
void sexp_free(struct s_exp *e){
	if( --(e->ref) < 1){
		free(e);
	}
}

/*
 * create s_exp from integer
 */
struct s_exp *integer2sexp(int i){
	struct s_exp *e = sexp_alloc();
	e->type = S_EXP_INTEGER;
	(e->u).integer = i;
	return e;
}

/*
 * create s_exp from character
 */
struct s_exp *character2sexp(char c){
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_CHARACTER;
	(e->u).character = c;
	return e;
}

/*
 * create s_exp from symbol
 */
struct s_exp *symbol2sexp(char *s){
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_SYMBOL;
	(e->u).symbol = s;
	return e;
}

/*
 * create s_exp from builtin
 */
struct s_exp *builtin2sexp(char *s){
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_BUILTIN;
	(e->u).builtin = s;
	return e;
}

/*
 * create s_exp from special
 */
struct s_exp *special2sexp(char *s){
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_SPECIAL;
	(e->u).special = s;
	return e;
}

/*
 * create s_exp from dotted pair
 */
struct s_exp *cons(struct s_exp *car, struct s_exp *cdr){
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_PAIR;
	(e->u).pair.car = car;
	(e->u).pair.cdr = cdr;
	return e;
}

/*
 * return true if the type is pair and the cdr is nil end.
 */
static int is_list(struct s_exp *e){
	if(e == nil)
		return 1;
	else if(e->type != S_EXP_PAIR)
		return 0;
	else
		return is_list(e->u.pair.cdr);
}

static void _write_type(struct s_exp *e){
	printf(" : ");
	if(e == nil)
		printf("nil");
	else if(e == sexp_t || e == sexp_f)
		printf("boolean");
	else if(e->type == S_EXP_INTEGER)
		printf("integer");
	else if(e->type == S_EXP_CHARACTER)
		printf("character");
	else if(e->type == S_EXP_SYMBOL)
		printf("symbol");
	else if(e->type == S_EXP_SPECIAL)
		printf("special");
	else if(e->type == S_EXP_BUILTIN)
		printf("builtin");
	else if(e->type == S_EXP_PAIR)
		if(is_list(e))
			printf("list");
		else
			printf("pair");
}

static void _write_sexp(struct s_exp *e, int d);

/*
 * d = 0 if highest position
 *     1 otherwise
 */
static void _write_list(struct s_exp *e, int d){
	if(d == 0)
		printf("(");

	_write_sexp(e->u.pair.car,1);
	if(e->u.pair.cdr == nil){
		printf(")");
	}else{
		printf(" ");
		_write_list(e->u.pair.cdr,1);
	}	
}

/*
 * d = 0 if highest position
 *     1 otherwise
 */
static void _write_sexp(struct s_exp *e, int d){

	if(e == nil){
		printf("nil");
	}else if(e == sexp_t){
		printf("#t");
	}else if(e == sexp_f){
		printf("#f");
	}else if(e->type == S_EXP_INTEGER){
		printf("%d",e->u.integer);
	}else if(e->type == S_EXP_CHARACTER){
		printf("%c",e->u.character);
	}else if(e->type == S_EXP_SYMBOL){
		printf("%s",e->u.symbol);
	}else if(e->type == S_EXP_BUILTIN){
		printf("%s",e->u.builtin);
	}else if(e->type == S_EXP_SPECIAL){
		printf("%s",e->u.special);
	}else if(is_list(e)){			/* for list */
		_write_list(e,0);
	}else{					/* dotted pair */
		printf("(");
		_write_sexp(e->u.pair.car,1);
		printf(" . ");
		_write_sexp(e->u.pair.cdr,1);
		printf(")");
	}
	if(d==0)
		_write_type(e);
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
		sexp_free(p);
		p = t;
	}
	return integer2sexp(s);
}

/*
 * minus function
 */
static struct s_exp *minus(struct s_exp *args){
	struct s_exp *p;
	struct s_exp *q,*t;
	int s = eval(args->u.pair.car)->u.integer;
	p = args->u.pair.cdr;
	sexp_free(args);

	while(p != nil){
		q = eval(p->u.pair.car);
		t = p->u.pair.cdr;
		sexp_free(p);
		p = t;
		s -= q->u.integer;
	}
	return integer2sexp(s);
}

/*
 * multi function
 */
static struct s_exp *multi(struct s_exp *args){
	struct s_exp *p = args;
	struct s_exp *q,*t;
	int s = 1;
	while(p != nil){
		q = eval(p->u.pair.car);
		s *= q->u.integer;
		t = p->u.pair.cdr;
		sexp_free(p);
		p = t;
	}
	return integer2sexp(s);
}

/*
 * divi function
 */
static struct s_exp *divi(struct s_exp *args){
	struct s_exp *p;
	struct s_exp *q,*t;
	int s = eval(args->u.pair.car)->u.integer;
	p = args->u.pair.cdr;
	sexp_free(args);

	while(p != nil){
		q = eval(p->u.pair.car);
		t = p->u.pair.cdr;
		sexp_free(p);
		p = t;
		s -= q->u.integer;
	}
	return integer2sexp(s);
}

/*
 * or function
 */
static struct s_exp *or(struct s_exp *args){
	struct s_exp *p, *q;

	if(args == nil) return sexp_f;

	p = args->u.pair.car;
	q = args->u.pair.cdr;
	sexp_free(args);
	return (p != sexp_f) ? sexp_t : or(q);
}

/*
 * and function
 */
static struct s_exp *and(struct s_exp *args){
	struct s_exp *p, *q;

	if(args == nil) return sexp_t;

	p = args->u.pair.car;
	q = args->u.pair.cdr;
	sexp_free(args);
	return (p == sexp_f) ? sexp_f : and(q);
}

/*
 * list function
 */
static struct s_exp *list(struct s_exp *args){
	if(args == nil)
		return nil;
	return cons(args->u.pair.car,list(args->u.pair.cdr));
}

/*
 * if special operation
 */
static struct s_exp *_if(struct s_exp *args){
	struct s_exp *p, *q, *r;
	p = args->u.pair.car;
	q = args->u.pair.cdr->u.pair.car;
	r = args->u.pair.cdr->u.pair.cdr->u.pair.car;
	if(eval(p) == sexp_f)
		return eval(r);
	else
		return eval(q);
}

/*
 * define special operation
 */
static struct s_exp *define(struct s_exp *args){
	struct s_exp *s = args->u.pair.car;
	struct s_exp *p = args->u.pair.cdr->u.pair.car;

	p = eval(p);
	
	st_insert(global_table, s->u.symbol, p);
	sexp_free(s);
	st_dump(global_table);
	return sexp_t;
}

/*
 * eval s expression
 *
 * 	(1) atom
 * 	(2) symbol
 * 	(3) special operation
 * 	(4) builtin function
 *
 */
struct s_exp *eval(struct s_exp *e){
	struct s_exp *car, *cdr;

	if(e == nil){					/* (1) nil */
		return nil;
	}else if(e == sexp_t || e == sexp_f){		/* (1) boolean */
		return e;
	}else if(e->type == S_EXP_INTEGER){		/* (1) integer */
		return e;
	}else if(e->type == S_EXP_CHARACTER){		/* (1) character */
		return e;
	}else if(e->type == S_EXP_SYMBOL){		/* (2) symbol */
		struct s_exp *p;
		if(!(p = st_find(global_table, e->u.symbol))){
			fprintf(stderr,"Error: undefined symbol \"%s\".\n", 
					e->u.symbol);
			return nil;
		}
		return p;
	}else if(e->type == S_EXP_BUILTIN){ 		/* (1) builtin */
		return e;
	}else if(e->type == S_EXP_SPECIAL){		/* (2) special */
		return e;
	}

	car = e->u.pair.car;
	cdr = e->u.pair.cdr;

	car = eval(car);
	if(car->type == S_EXP_SPECIAL){				/* (3) special operation */
		if(!strcmp(car->u.symbol, "quote")){
			return cdr->u.pair.car;
		}else if(!strcmp(car->u.symbol,"if")){
			return _if(cdr);
		}else if(!strcmp(car->u.symbol,"define")){
			printf("defineeee\n");
			return define(cdr);
		}
	}else if(car->type == S_EXP_BUILTIN){			/* (4) builtin function apply */
		sexp_free(e);
		return apply(car,cdr);
	}
}

/*
 * apply basic function
 */
struct s_exp *apply(struct s_exp *func, struct s_exp *args){
	char *f_name = func->u.symbol;
	struct s_exp *p, *q;

	/* p: args[0], q: args[1] */
	p = args->u.pair.car;
	if(args->u.pair.cdr != nil)
		q = args->u.pair.cdr->u.pair.car;


	if(!strcmp(f_name,"+")){
		return add(args);
	}else if(!strcmp(f_name,"-")){
		return minus(args);
	}else if(!strcmp(f_name,"*")){
		return multi(args);
	}else if(!strcmp(f_name,"/")){
		return divi(args);
	}else if(!strcmp(f_name,"cons")){
		return cons(p,q);
	}else if(!strcmp(f_name,"car")){
		return p->u.pair.car;
	}else if(!strcmp(f_name,"cdr")){
		return p->u.pair.cdr;
	}else if(!strcmp(f_name,"list")){
		return list(args);
	}else if(!strcmp(f_name,"eval")){
		return eval(p);
	}else if(!strcmp(f_name,"eq?")){
		return (p==q) ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,"atom?")){
		if(p==nil || p==sexp_t || p==sexp_f 
			|| p->type == S_EXP_INTEGER 
			|| p->type == S_EXP_CHARACTER
			|| p->type == S_EXP_SYMBOL)
			return sexp_t;
		else
			return sexp_f;
	}else if(!strcmp(f_name,"nil?")){
		return (p==nil) ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,"or")){
		return or(args);
	}else if(!strcmp(f_name,"and")){
		return and(args);
	}else if(!strcmp(f_name,"not")){
		return p == sexp_f ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,"=")){
		return (p->u.integer == q->u.integer) ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,">")){
		return (p->u.integer > q->u.integer) ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,"<")){
		return (p->u.integer < q->u.integer) ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,"<=")){
		return (p->u.integer <= q->u.integer) ? sexp_t : sexp_f;
	}else if(!strcmp(f_name,">=")){
		return (p->u.integer >= q->u.integer) ? sexp_t : sexp_f;
	}
	fprintf(stderr, "undefied variable %s.",f_name);
	return nil;
}




int main(int argc, char **argv) {
	char *p;

	global_table = st_create(NULL);
	st_init(global_table);

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
	st_destory(global_table);
	return 0;
}
