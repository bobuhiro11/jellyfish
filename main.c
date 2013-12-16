#include "common.h"

int interactive;
int linenum=0;
struct symbol_table *global_table;

void
yyerror(char* s)
{
	printf("Error: %s\n", s);
}

void
prompt()
{
	if(interactive)
		printf("[%d]> ",linenum++);
}

void
prompt_multi()
{
	if(interactive)
		printf("...   ");
}

int
yywrap()
{
	return 1;
}

/*
 * alloc new sexp object.
 * return sexp object if success, NULL otherwise.
 */
struct s_exp *
sexp_alloc()
{
	struct s_exp *rc =  (struct s_exp*)malloc(sizeof(struct s_exp));
	if(!rc)
		return NULL;
	rc->ref = 1;
	return rc;
}

/*
 * copy sexp object.
 */
struct s_exp *
sexp_copy(struct s_exp *e)
{
	if(is_singleton(e))
		return e;

	struct s_exp *p = sexp_alloc();
	int len;

	if(e->type == S_EXP_PAIR){
		p->type = S_EXP_PAIR;
		p->u.pair.car = sexp_copy(e->u.pair.car);
		p->u.pair.cdr = sexp_copy(e->u.pair.cdr);
	}else if(e->type == S_EXP_INTEGER){
		p->type = S_EXP_INTEGER;
		p->u.integer = e->u.integer;
	}else if(e->type == S_EXP_CHARACTER){
		p->type = S_EXP_CHARACTER;
		p->u.character = e->u.character;
	}else if(e->type == S_EXP_STRING){
		p->type = S_EXP_STRING;
		p->u.string = e->u.string;
		len = strlen( e->u.string );
		p->u.string = malloc(len+1);
		memset(p->u.string, 0, len+1);
		strncpy(p->u.string, e->u.string, len);
	}else if(e->type == S_EXP_SYMBOL){
		p->type = S_EXP_SYMBOL;
		len = strlen( e->u.symbol );
		p->u.symbol = malloc(len+1);
		memset(p->u.symbol, 0, len+1);
		strncpy(p->u.symbol, e->u.symbol, len);
	}else if(e->type == S_EXP_BUILTIN){
		p->type = S_EXP_SYMBOL;
		p->u.symbol = e->u.symbol;
	}else if(e->type == S_EXP_SPECIAL){
		p->type = S_EXP_SYMBOL;
		p->u.special = e->u.special;
	}else if(e->type == S_EXP_CLOJURE){
		p->type = S_EXP_CLOJURE;
		p->u.pair.car = sexp_copy(e->u.pair.car);
		p->u.pair.cdr = sexp_copy(e->u.pair.cdr);
	}

	return p;
}

/*
 * ref new sexp object.
 * return sexp object if success, NULL otherwise.
 */
struct s_exp *
sexp_ref(struct s_exp *e)
{
	if(e != nil && e!= sexp_t && e!= sexp_f)
		e->ref++;
	return e;
}

/*
 * free sexp object if reference count is 0.
 */
void
sexp_free(struct s_exp *e)
{
	if( --(e->ref) < 1)
		free(e);
}

/*
 * create s_exp from integer
 */
struct s_exp *
integer2sexp(int i)
{
	struct s_exp *e = sexp_alloc();
	e->type = S_EXP_INTEGER;
	(e->u).integer = i;
	return e;
}

/*
 * create s_exp from character
 */
struct s_exp *
character2sexp(char c)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_CHARACTER;
	(e->u).character = c;
	return e;
}

/*
 * create s_exp from symbol
 */
struct s_exp *
symbol2sexp(char *s)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_SYMBOL;
	(e->u).symbol = s;
	return e;
}

/*
 * create s_exp from string
 */
struct s_exp *
string2sexp(char *s)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_STRING;
	(e->u).string = s;
	return e;
}

/*
 * create s_exp from builtin
 */
struct s_exp *
builtin2sexp(char *s)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_BUILTIN;
	(e->u).builtin = s;
	return e;
}

/*
 * create s_exp from special
 */
struct s_exp *
special2sexp(char *s)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_SPECIAL;
	(e->u).special = s;
	return e;
}

/*
 * create s_exp from clojure
 */
struct s_exp *
clojure2sexp(struct s_exp *p)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_CLOJURE;
	(e->u).pair.car = p->u.pair.car;
	(e->u).pair.cdr = p->u.pair.cdr;
	return e;
}

/*
 * create s_exp from dotted pair
 */
struct s_exp *
cons(struct s_exp *car, struct s_exp *cdr)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_PAIR;
	(e->u).pair.car = car;
	(e->u).pair.cdr = cdr;
	return e;
}

/*
 * append two s_exp object
 */
struct s_exp *
append(struct s_exp *exp1, struct s_exp *exp2)
{
	struct s_exp *rc, *e;

	if(exp1 == nil)
		return sexp_copy(exp2);

	rc = e = sexp_copy(exp1);
	while(e->u.pair.cdr != nil)
		e = e->u.pair.cdr;
	e->u.pair.cdr = sexp_copy(exp2);

	return rc;
}

/*
 * begin
 * return sexp_undef
 */
struct s_exp *
begin(struct s_exp *args)
{
	struct s_exp *rc=sexp_undef;
	struct s_exp *e = args;

	while(e != nil){
		rc = eval(e->u.pair.car);
		e = e->u.pair.cdr;
	}
	return rc;
}

/*
 * return true if the type is pair and the cdr is nil end.
 */
static int
is_list(struct s_exp *e)
{
	if(e == nil)
		return 1;
	else if(e->type != S_EXP_PAIR)
		return 0;
	else
		return is_list(e->u.pair.cdr);
}

static void
_write_type(struct s_exp *e)
{
	printf(" : ");
	if(e == nil)
		printf("nil");
	else if(e == sexp_undef)
		printf("undef");
	else if(e == sexp_t || e == sexp_f)
		printf("boolean");
	else if(e->type == S_EXP_INTEGER)
		printf("integer");
	else if(e->type == S_EXP_CHARACTER)
		printf("character");
	else if(e->type == S_EXP_STRING)
		printf("string");
	else if(e->type == S_EXP_SYMBOL)
		printf("symbol");
	else if(e->type == S_EXP_SPECIAL)
		printf("special");
	else if(e->type == S_EXP_BUILTIN)
		printf("builtin");
	else if(e->type == S_EXP_CLOJURE)
		printf("clojure");
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
static void
_write_list(struct s_exp *e, int d)
{
	if(!d)
		printf("(");

	_write_sexp(e->u.pair.car,1);

	if(e->u.pair.cdr == nil)
		printf(")");
	else{
		printf(" ");
		_write_list(e->u.pair.cdr,1);
	}
}

/*
 * d = 0 if write type
 *     1 otherwise
 */
static void
_write_sexp(struct s_exp *e, int d)
{

	if(e == nil)
		printf("nil");
	else if(e == sexp_undef)
	       printf("undef");
	else if(e == sexp_t)
	       printf("#t");
	else if(e == sexp_f)
	       printf("#f");
	else if(e->type == S_EXP_INTEGER)
	       printf("%d",e->u.integer);
	else if(e->type == S_EXP_CHARACTER)
	       printf("%c",e->u.character);
	else if(e->type == S_EXP_STRING)
	       printf("%s",e->u.string);
	else if(e->type == S_EXP_SYMBOL)
	       printf("%s",e->u.symbol);
	else if(e->type == S_EXP_BUILTIN)
	       printf("%s",e->u.builtin);
	else if(e->type == S_EXP_SPECIAL)
	       printf("%s",e->u.special);
	else if(e->type == S_EXP_CLOJURE)
	       _write_list(e,0);
	else if(is_list(e))			/* for list */
	       _write_list(e,0);
	else{					/* dotted pair */
		printf("(");
		_write_sexp(e->u.pair.car,1);
		printf(" . ");
		_write_sexp(e->u.pair.cdr,1);
		printf(")");
	}

	if(!d)
		_write_type(e);
}

void
write_sexp(struct s_exp *e)
{
	_write_sexp(e,0);
}

struct s_exp *
display(struct s_exp *args)
{
	struct s_exp *p = args;

	while(p != nil){
		_write_sexp(p->u.pair.car,1);
		p = p->u.pair.cdr;
	}

	if(interactive)
		printf("\n");
	return sexp_undef;
}

static struct s_exp *
add(struct s_exp *args)
{
	struct s_exp *p = args;
	struct s_exp *q;
	int s = 0;

	while(p != nil){
		s += p->u.pair.car->u.integer;
		p = p->u.pair.cdr;
	}
	return integer2sexp(s);
}

static struct s_exp *
minus(struct s_exp *args)
{
	struct s_exp *p = args->u.pair.cdr;
	int s = eval(args->u.pair.car)->u.integer;

	while(p != nil){
		s -= p->u.pair.car->u.integer;
		p = p->u.pair.cdr;
	}
	return integer2sexp(s);
}

static struct s_exp *
multi(struct s_exp *args)
{
	struct s_exp *p = args;
	int s = 1;

	while(p != nil){
		s *= p->u.pair.car->u.integer;
		p = p->u.pair.cdr;
	}
	return integer2sexp(s);
}

static struct s_exp *
divi(struct s_exp *args)
{
	struct s_exp *p = args->u.pair.cdr;
	int s = args->u.pair.car->u.integer;

	while(p != nil){
		s /= p->u.pair.car->u.integer;
		p = p->u.pair.cdr;
	}
	return integer2sexp(s);
}

static struct s_exp *
modulo(struct s_exp *args)
{
	int x = args->u.pair.car->u.integer;
	int y = args->u.pair.cdr->u.pair.car->u.integer;

	return integer2sexp(x % y);
}

static struct s_exp *
or(struct s_exp *args)
{
	struct s_exp *p, *q;

	if(args == nil)
		return sexp_f;

	p = args->u.pair.car;
	q = args->u.pair.cdr;

	return (p != sexp_f) ? sexp_t : or(q);
}

static struct s_exp *
and(struct s_exp *args)
{
	struct s_exp *p, *q;

	if(args == nil)
		return sexp_t;

	p = args->u.pair.car;
	q = args->u.pair.cdr;

	return (p == sexp_f) ? sexp_f : and(q);
}

static struct s_exp *
list(struct s_exp *args)
{
	if(args == nil)
		return nil;

	return cons(args->u.pair.car,list(args->u.pair.cdr));
}

static struct s_exp *
_if(struct s_exp *args)
{
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
static struct s_exp *define(struct s_exp *args)
{
	struct s_exp *s = args->u.pair.car;
	struct s_exp *p = sexp_ref(args->u.pair.cdr->u.pair.car);
	struct s_exp *q;

	if(s->type == S_EXP_PAIR){	/* syntax sugar */
		q = s->u.pair.cdr;
		p = cons(q, cons(p,nil));
		p = clojure2sexp(p);
		s = s->u.pair.car;
	}

	p = eval(p);
	st_insert(global_table, s->u.symbol, p);

	return sexp_undef;
}

struct s_exp *eval(struct s_exp *e)
{
	struct s_exp *car, *cdr;

	//printf("eval > ");write_sexp(e);printf("\n");
	//st_dump(global_table);printf("\n\n");

	if(e == nil || e == sexp_t || e == sexp_f || e == sexp_undef)
		return e;

	switch(e->type){
		case S_EXP_INTEGER:
		case S_EXP_CHARACTER:
		case S_EXP_STRING:
		case S_EXP_BUILTIN:
		case S_EXP_SPECIAL:
		case S_EXP_CLOJURE:
			return e;
		case S_EXP_SYMBOL:
			return st_find(global_table, e->u.symbol);
	}

	car = e->u.pair.car;
	cdr = e->u.pair.cdr;

	car = eval(car);
	switch(car->type){
		case S_EXP_SPECIAL:	return apply_special(car,cdr);
		case S_EXP_CLOJURE:	return apply_clojure(car,cdr);
		case S_EXP_BUILTIN:	return apply_builtin(car,cdr);
	}

	return nil;

}

struct s_exp *
apply_special(struct s_exp *car, struct s_exp *cdr)
{
	struct s_exp *rc = nil;

	if(!strcmp(car->u.symbol, "quote"))		rc = cdr->u.pair.car;
	else if(!strcmp(car->u.symbol,"if"))		rc = _if(cdr);
	else if(!strcmp(car->u.symbol,"define"))	rc = define(cdr);
	else if(!strcmp(car->u.symbol,"symbols"))	rc = st_dump(global_table);
	else if(!strcmp(car->u.symbol,"lambda"))	rc = clojure2sexp(cdr);
	else if(!strcmp(car->u.symbol,"begin"))		rc = begin(cdr);

	return rc;
}

struct s_exp *
apply_clojure(struct s_exp *clojure, struct s_exp *args)
{
	struct symbol_table *p;
	struct s_exp *e1, *e2, *rc;

	p = st_create();				/* new scope*/

	e1 = clojure->u.pair.car;			/* insert and eval arguments */
	e2 = args;

	while(e1 && e2){
		st_insert(p, e1->u.pair.car->u.symbol,eval(e2->u.pair.car));

		e1 = e1->u.pair.cdr;
		e2 = e2->u.pair.cdr;
	}

	p->next = global_table;				/* add new scope */
	global_table = p;

	rc = sexp_copy(clojure->u.pair.cdr->u.pair.car);/* copy clojure expression and eval */
	rc = eval(rc);

	p = global_table;				/* remove new scope */
	global_table = global_table->next;
	st_destory(p);

	return rc;
}

struct s_exp *
apply_builtin(struct s_exp *func, struct s_exp *args)
{
	char *f_name = func->u.symbol;
	struct s_exp *p, *q, *rc=nil;

	/* eval args */
	p = args;
	while(p != nil){
		p->u.pair.car = eval(p->u.pair.car);
		p = p->u.pair.cdr;
	}

	if(args){
		p = args->u.pair.car;
		if(args->u.pair.cdr)
			q = args->u.pair.cdr->u.pair.car;
	}

	if(!strcmp(f_name,"+"))			rc = add(args);
	else if(!strcmp(f_name,"-"))		rc = minus(args);
	else if(!strcmp(f_name,"*"))		rc = multi(args);
	else if(!strcmp(f_name,"/"))		rc = divi(args);
	else if(!strcmp(f_name,"modulo"))	rc = modulo(args);
	else if(!strcmp(f_name,"cons"))		rc = cons(p,q);
	else if(!strcmp(f_name,"append"))	rc = append(p,q);
	else if(!strcmp(f_name,"car"))		rc = p->u.pair.car;
	else if(!strcmp(f_name,"cdr"))		rc = p->u.pair.cdr;
	else if(!strcmp(f_name,"list"))		rc = list(args);
	else if(!strcmp(f_name,"eval"))		rc = eval(p);
	else if(!strcmp(f_name,"display"))	rc = display(args);
	else if(!strcmp(f_name,"newline")){
		putc('\n', stdout);
		return sexp_undef;
	}else if(!strcmp(f_name,"eq?"))		rc = p==q ? sexp_t : sexp_f;
	else if(!strcmp(f_name,"atom?")){
		if(p==nil || p==sexp_t || p==sexp_f
			|| p->type == S_EXP_INTEGER
			|| p->type == S_EXP_CHARACTER
			|| p->type == S_EXP_SYMBOL)
			return sexp_t;
		else
			return sexp_f;
	}else if(!strcmp(f_name,"nil?"))	rc = p==nil ? sexp_t : sexp_f;
	else if(!strcmp(f_name,"null?"))	rc = p==nil ? sexp_t : sexp_f;
	else if(!strcmp(f_name,"or"))		rc = or(args);
	else if(!strcmp(f_name,"and"))		rc = and(args);
	else if(!strcmp(f_name,"not"))		rc = p == sexp_f ? sexp_t : sexp_f;
	else if(!strcmp(f_name,"="))		rc = p->u.integer == q->u.integer ? sexp_t : sexp_f;
	else if(!strcmp(f_name,">"))		rc = p->u.integer > q->u.integer ? sexp_t : sexp_f;
	else if(!strcmp(f_name,"<"))		rc = p->u.integer < q->u.integer ? sexp_t : sexp_f;
	else if(!strcmp(f_name,"<="))		rc = p->u.integer <= q->u.integer ? sexp_t : sexp_f;
	else if(!strcmp(f_name,">="))		rc = p->u.integer >= q->u.integer ? sexp_t : sexp_f;

	return rc;
}




int main(int argc, char **argv)
{
	global_table = st_create();
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
