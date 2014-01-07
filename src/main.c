#include "../include/common.h"

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

void
debug(const char *mes, struct s_exp *e){
#ifndef NOFREE
	printf("%s: ",mes);
	write_sexp(e);
	printf("\n");
#endif
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
	return rc;
}

/*
 * copy sexp object.
 */
struct s_exp *
sexp_copy(struct s_exp *e)
{
	int len;
	struct s_exp *p;

	if(is_singleton(e))
		return e;

	p = sexp_alloc();
	p->type = e->type;

	switch(e->type){
		case S_EXP_CHARACTER:
			p->u.character = e->u.character;
			break;
		case S_EXP_INTEGER:
			p->u.integer = e->u.integer;
			break;
		case S_EXP_PAIR:
		case S_EXP_CLOJURE:
			p->u.pair.car = sexp_copy(e->u.pair.car);
			p->u.pair.cdr = sexp_copy(e->u.pair.cdr);
			break;
		case S_EXP_STRING:
		case S_EXP_SYMBOL:
			len = strlen( e->u.obj );
			p->u.obj = malloc(len+1);
			memset(p->u.obj, 0, len+1);
			strncpy(p->u.obj, e->u.obj, len);
			break;
		case S_EXP_BUILTIN:
		case S_EXP_SPECIAL:
			p->u.obj = e->u.obj;
			break;
	}

	return p;
}

/*
 * free sexp object
 *
 * @rec 1 if recursive, 0 otherwise
 */
void
sexp_free(struct s_exp *e, int rec)
{
#ifdef NOFREE
	return;
#endif
	if(is_singleton(e))
		return;

	if(e->type == S_EXP_STRING || e->type == S_EXP_SYMBOL)
		free(e->u.obj);
	else if((e->type == S_EXP_PAIR || e->type == S_EXP_CLOJURE) && rec){
		sexp_free(e->u.pair.car,1);
		sexp_free(e->u.pair.cdr,1);
	}

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
	e->u.obj = s;
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
	e->u.obj = s;
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
	e->u.obj = s;
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
	e->u.obj = s;
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
	sexp_free(p,0);
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

struct s_exp *
jf_cons(struct s_exp *args)
{
	struct s_exp *e =  sexp_alloc();
	e->type = S_EXP_PAIR;
	(e->u).pair.car = args->u.pair.car;
	(e->u).pair.cdr = args->u.pair.cdr->u.pair.car;

	sexp_free(args->u.pair.cdr->u.pair.cdr,1);
	sexp_free(args->u.pair.cdr,0);
	sexp_free(args,0);

	return e;
}

/*
 * append two s_exp object
 */
struct s_exp *
jf_append(struct s_exp *args)
{
	struct s_exp *rc, *e;
	struct s_exp *exp1,*exp2;

	exp1 = args->u.pair.car;
	exp2 = args->u.pair.cdr->u.pair.car;

	if(exp1 == nil){
		sexp_free(args->u.pair.cdr,0);
		sexp_free(args,0);
		return exp2;
	}

	rc = e = exp1;
	while(e->u.pair.cdr != nil)
		e = e->u.pair.cdr;
	e->u.pair.cdr = exp2;

	sexp_free(args->u.pair.cdr,0);
	sexp_free(args,0);
	return rc;
}

/*
 * begin
 * return sexp_undef
 */
struct s_exp *
jf_begin(struct s_exp *args)
{
	struct s_exp *rc=sexp_undef;
	struct s_exp *e = args, *q;

	while(e != nil){
		sexp_free(rc,1);
		rc = eval(e->u.pair.car);
		q = e->u.pair.cdr;
		sexp_free(e,0);
		e = q;
	}
	return rc;
}

/*
 * return true if the type is pair and the cdr is nil end.
 */
static int
is_list(struct s_exp *e)
{
	if(e == nil)			return 1;
	else if(e->type != S_EXP_PAIR)	return 0;
	else 				return is_list(e->u.pair.cdr);
}

static void
_write_type(struct s_exp *e)
{
	printf(" : ");

	if(e == nil)				printf("nil");
	else if(e == sexp_undef)		printf("undef");
	else if(e == sexp_t || e == sexp_f)	printf("boolean");
	else if(e->type == S_EXP_INTEGER)	printf("integer");
	else if(e->type == S_EXP_CHARACTER)	printf("character");
	else if(e->type == S_EXP_STRING)	printf("string");
	else if(e->type == S_EXP_SYMBOL)	printf("symbol");
	else if(e->type == S_EXP_SPECIAL)	printf("special");
	else if(e->type == S_EXP_BUILTIN)	printf("builtin");
	else if(e->type == S_EXP_CLOJURE)	printf("clojure");
	else if(e->type == S_EXP_PAIR)		is_list(e) ? printf("list") : printf("pair");
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

static void
_write_pair(struct s_exp *e){
	printf("(");
	_write_sexp(e->u.pair.car,1);
	printf(" . ");
	_write_sexp(e->u.pair.cdr,1);
	printf(")");
}

/*
 * d = 0 if write type
 *     1 otherwise
 */
static void
_write_sexp(struct s_exp *e, int d)
{
	if(e == nil)				printf("nil");
	else if(e == sexp_undef)		printf("undef");
	else if(e == sexp_t)			printf("#t");
	else if(e == sexp_f)			printf("#f");
	else if(e->type == S_EXP_INTEGER)	printf("%d",e->u.integer);
	else if(e->type == S_EXP_CHARACTER)	printf("%c",e->u.character);
	else if(e->type == S_EXP_STRING
		|| e->type == S_EXP_SYMBOL
		|| e->type == S_EXP_BUILTIN
		|| e->type == S_EXP_SPECIAL)	printf("%s",e->u.obj);
	else if(e->type == S_EXP_CLOJURE)	_write_list(e,0);
	else if(is_list(e))			_write_list(e,0);
	else					_write_pair(e);


	if(!d)					_write_type(e);
}

void
write_sexp(struct s_exp *e)
{
	_write_sexp(e,0);
}

static struct s_exp *
jf_list2string(struct s_exp *args)
{
	int n=0;
	char *str;
	struct s_exp *p = args->u.pair.car;

	while(p!=nil){
		n++;
		p = p->u.pair.cdr;
	}
	str = (char*)malloc(sizeof(n+1));

	p = args->u.pair.car;
	n = 0;
	while(p!=nil){
		str[n++] = p->u.pair.car->u.character;
		p = p->u.pair.cdr;
	}
	str[n] = '\0';

	sexp_free(args,1);
	return string2sexp(str);
}

static struct s_exp *
jf_display(struct s_exp *args)
{
	struct s_exp *p = args, *q;

	while(p != nil){
		_write_sexp(p->u.pair.car,1);
		sexp_free(p->u.pair.car,1);
		q = p->u.pair.cdr;
		sexp_free(p,0);
		p = q;
	}

	if(interactive)
		printf("\n");
	return sexp_undef;
}

static struct s_exp *
jf_newline(struct s_exp *args)
{
	sexp_free(args,1);
	printf("\n");
	return sexp_undef;
}

static struct s_exp *
jf_null(struct s_exp *args)
{
	struct s_exp *p = args->u.pair.car;
	struct s_exp *rc;

	if(p == nil)	rc = sexp_t;
	else 		rc = sexp_f;

	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_symbols(struct s_exp *args)
{
	st_dump(global_table);
	sexp_free(args,1);
	return sexp_undef;
}

static struct s_exp *
jf_car(struct s_exp *args)
{
	struct s_exp *rc = sexp_copy(args->u.pair.car->u.pair.car);
	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_cdr(struct s_exp *args)
{
	struct s_exp *rc = sexp_copy(args->u.pair.car->u.pair.cdr);
	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_atom(struct s_exp *args)
{
	struct s_exp *p = args->u.pair.car;
	struct s_exp *rc;

	if(is_singleton(p)
		|| p->type == S_EXP_INTEGER
		|| p->type == S_EXP_CHARACTER
		|| p->type == S_EXP_SYMBOL)
		rc = sexp_t;
	else
		rc = sexp_f;

	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_cmp(struct s_exp *args, int flag)
{
	int r;
	struct s_exp *p;
	struct s_exp *q;
	struct s_exp *rc;

	p = args->u.pair.car;
	q = args->u.pair.cdr->u.pair.car;

	switch(flag){
		case CMP_LESS:
			r = p->u.integer < q->u.integer;
			break;
		case CMP_LESS_EQUAL:
			r = p->u.integer <= q->u.integer;
			break;
		case CMP_GREATER_EQUAL:
			r = p->u.integer >= q->u.integer;
			break;
		case CMP_GREATER:
			r = p->u.integer > q->u.integer;
			break;
		case CMP_EQUAL:
			r = p->u.integer == q->u.integer;
			break;
	}

	rc = r ? sexp_t : sexp_f;
	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_add(struct s_exp *args)
{
	struct s_exp *p = args;
	struct s_exp *q;
	int s = 0;

	while(p != nil){
		s += p->u.pair.car->u.integer;
		sexp_free(p->u.pair.car,1);
		q = p->u.pair.cdr;
		sexp_free(p,0);
		p = q;
	}
	return integer2sexp(s);
}

static struct s_exp *
jf_minus(struct s_exp *args)
{
	struct s_exp *q,*p = args->u.pair.cdr;
	int s = args->u.pair.car->u.integer;

	while(p != nil){
		s -= p->u.pair.car->u.integer;
		sexp_free(p->u.pair.car,1);
		q = p->u.pair.cdr;
		sexp_free(p,0);
		p = q;
	}
	sexp_free(args->u.pair.car,1);
	sexp_free(args,0);
	return integer2sexp(s);
}

static struct s_exp *
jf_multi(struct s_exp *args)
{
	struct s_exp *p = args;
	struct s_exp *q;
	int s = 1;

	while(p != nil){
		s *= p->u.pair.car->u.integer;
		sexp_free(p->u.pair.car,1);
		q = p->u.pair.cdr;
		sexp_free(p,0);
		p = q;
	}
	return integer2sexp(s);
}

static struct s_exp *
jf_divi(struct s_exp *args)
{
	struct s_exp *q,*p = args->u.pair.cdr;
	int s = args->u.pair.car->u.integer;

	while(p != nil){
		s /= p->u.pair.car->u.integer;
		sexp_free(p->u.pair.car,1);
		q = p->u.pair.cdr;
		sexp_free(p,0);
		p = q;
	}
	sexp_free(args->u.pair.car,1);
	sexp_free(args,0);
	return integer2sexp(s);
}

static struct s_exp *
jf_modulo(struct s_exp *args)
{
	struct s_exp *rc;
	int x = args->u.pair.car->u.integer;
	int y = args->u.pair.cdr->u.pair.car->u.integer;

	rc = integer2sexp(x % y);
	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_or(struct s_exp *args)
{
	struct s_exp *p, *q;

	if(args == nil)
		return sexp_f;

	p = args->u.pair.car;
	q = args->u.pair.cdr;

	if(p != sexp_f){
		sexp_free(args,1);
		return sexp_t;
	}else{
		sexp_free(p,1);
		sexp_free(args,0);
		return jf_or(q);
	}

}

static struct s_exp *
jf_eq(struct s_exp *args)
{
	struct s_exp *p, *q, *rc;

	p = args->u.pair.car;
	q = args->u.pair.cdr->u.pair.car;

	rc = p==q ? sexp_t: sexp_f;
	sexp_free(args,1);

	return rc;
}

static int
_equal(struct s_exp *p, struct s_exp *q)
{
	int rc;
	int pt, qt;

	if(p==nil || p==sexp_t || p==sexp_f || p==sexp_undef){
		return p==q ? 1 : 0;
	}

	pt = p->type;
	qt = q->type;

	if(pt != qt)
		rc = 1;
	else if(pt == S_EXP_PAIR || pt == S_EXP_CLOJURE){
		if(p->u.pair.cdr == nil && q->u.pair.cdr == nil)
			rc = _equal(p->u.pair.car, q->u.pair.car);
		else if(p->u.pair.cdr != nil && q->u.pair.cdr != nil)
			rc = _equal(p->u.pair.car, q->u.pair.car)
				&& _equal(p->u.pair.cdr, q->u.pair.cdr);
		else
			rc = 0;
	}else if(pt == S_EXP_INTEGER)
		rc = p->u.integer == q->u.integer;
	else if(pt == S_EXP_CHARACTER)
		rc = p->u.character == q->u.character;
	else
		rc = !strcmp(p->u.obj,q->u.obj);

	return rc;
}

static struct s_exp *
jf_equal(struct s_exp *args)
{
	struct s_exp *p, *q, *rc;

	p = args->u.pair.car;
	q = args->u.pair.cdr->u.pair.car;

	rc = _equal(p,q) ? sexp_t : sexp_f;
	sexp_free(args,1);

	return rc;
}

static struct s_exp *
jf_and(struct s_exp *args)
{
	struct s_exp *p, *q;

	if(args == nil)
		return sexp_t;

	p = args->u.pair.car;
	q = args->u.pair.cdr;

	if(p == sexp_f){
		sexp_free(args,1);
		return sexp_f;
	}else{
		sexp_free(p,1);
		sexp_free(args,0);
		return jf_and(q);
	}
}

static struct s_exp *
jf_not(struct s_exp *args)
{
	struct s_exp *p, *rc;

	p = args->u.pair.car;
	if(p != sexp_f)
		rc = sexp_f;
	else
		rc = sexp_t;

	sexp_free(args,1);
	return rc;
}

static struct s_exp *
jf_list(struct s_exp *args)
{
	struct s_exp *rc;
	if(args == nil)
		return nil;

	rc = cons(args->u.pair.car,jf_list(args->u.pair.cdr));
	sexp_free(args,0);
	return rc;
}

static struct s_exp *
jf_if(struct s_exp *args)
{
	struct s_exp *p, *q, *r;
	p = args->u.pair.car;
	q = args->u.pair.cdr->u.pair.car;
	r = args->u.pair.cdr->u.pair.cdr->u.pair.car;

	sexp_free(args->u.pair.cdr->u.pair.cdr->u.pair.cdr,1);
	sexp_free(args->u.pair.cdr->u.pair.cdr,0);
	sexp_free(args->u.pair.cdr,0);
	sexp_free(args,0);

	if(eval(p) == sexp_f){
		sexp_free(q,1);
		return eval(r);
	}else{
		sexp_free(r,1);
		return eval(q);
	}
}

static struct s_exp *
jf_quote(struct s_exp *args)
{
	struct s_exp *rc;

	rc = args->u.pair.car;
	sexp_free(args->u.pair.cdr,1);
	sexp_free(args,0);

	return rc;
}

/*
 * define special operation
 */
static struct s_exp *
jf_define(struct s_exp *args)
{
	struct s_exp *s = args->u.pair.car;
	struct s_exp *p = args->u.pair.cdr->u.pair.car;
	struct s_exp *q;

	if(s->type == S_EXP_PAIR){	/* syntax sugar */
		q = s->u.pair.cdr;
		p = cons(q, cons(p,nil));
		p = clojure2sexp(p);
		s->u.pair.cdr = nil;
		s = s->u.pair.car;
	}

	sexp_free(st_find(global_table, s->u.obj),1);
	p = eval(p);
	st_insert(global_table, s->u.obj, p);

	sexp_free(args->u.pair.car,1);
	sexp_free(args->u.pair.cdr->u.pair.cdr,1);
	sexp_free(args->u.pair.cdr,0);
	sexp_free(args,0);

	return sexp_undef;
}

struct s_exp *
js_eval(struct s_exp *args)
{
	struct s_exp *p, *rc;

	p = args->u.pair.car;
	rc = eval(p);

	sexp_free(args->u.pair.cdr,1);
	sexp_free(args,0);

	return rc;
}

struct s_exp *
eval(struct s_exp *e)
{
	struct s_exp *car, *cdr, *q, *p;

	if(is_singleton(e))
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
			q = st_find(global_table, e->u.obj);
			p = sexp_copy(q);
			sexp_free(e,1);
			return p;
	}

	car = e->u.pair.car;
	cdr = e->u.pair.cdr;
	sexp_free(e,0);

	q = eval(car);
	car = q;

	switch(car->type){
		case S_EXP_SPECIAL:	return jf_apply_special(car,cdr);
		case S_EXP_CLOJURE:	return jf_apply_clojure(car,cdr);
		case S_EXP_BUILTIN:	return jf_apply_builtin(car,cdr);
	}

	return nil;

}

struct s_exp *
jf_apply_special(struct s_exp *car, struct s_exp *cdr)
{
	struct s_exp *rc = nil;

	if(!strcmp(car->u.obj, "quote"))		rc = jf_quote(cdr);
	else if(!strcmp(car->u.obj,"if"))		rc = jf_if(cdr);
	else if(!strcmp(car->u.obj,"define"))		rc = jf_define(cdr);
	else if(!strcmp(car->u.obj,"symbols"))		rc = jf_symbols(cdr);
	else if(!strcmp(car->u.obj,"lambda"))		rc = clojure2sexp(cdr);
	else if(!strcmp(car->u.obj,"begin"))		rc = jf_begin(cdr);

	sexp_free(car,1);

	return rc;
}

struct s_exp *
jf_apply_clojure(struct s_exp *clojure, struct s_exp *args)
{
	struct symbol_table *p;
	struct s_exp *e1, *e2, *rc;
	struct s_exp *q;

	p = st_create();				/* new scope*/

	e1 = clojure->u.pair.car;			/* insert and eval arguments */
	e2 = args;

	while(e1 && e2){
		st_insert(p, e1->u.pair.car->u.obj,eval(e2->u.pair.car));

		q = e1->u.pair.cdr;
		sexp_free(e1->u.pair.car,1);
		sexp_free(e1,0);
		e1 = q;

		q = e2->u.pair.cdr;
		sexp_free(e2,0);
		e2 = q;
	}

	p->next = global_table;				/* add new scope */
	global_table = p;


	/*
	 * copy and eval clojure body.
	 *
	 * if we use sexp_ref() instead of sexp_copy(),
	 * the clojure call does'nt stop.(especially a case of recursive call)
	 */
	rc = sexp_copy(clojure->u.pair.cdr->u.pair.car);
	rc = eval(rc);

	p = global_table;				/* remove new scope */
	global_table = global_table->next;
	st_destory(p);

	sexp_free(clojure->u.pair.cdr->u.pair.car,1);
	sexp_free(clojure->u.pair.cdr,0);
	sexp_free(clojure,0);
	return rc;
}

struct s_exp *
jf_apply_builtin(struct s_exp *func, struct s_exp *args)
{
	char *f_name = func->u.obj;
	struct s_exp *p, *q, *rc=nil;


	/* eval args */
	p = args;
	while(p != nil){
		p->u.pair.car = eval(p->u.pair.car);
		p = p->u.pair.cdr;
	}

	if(!strcmp(f_name,"+"))			rc = jf_add(args);
	else if(!strcmp(f_name,"-"))		rc = jf_minus(args);
	else if(!strcmp(f_name,"*"))		rc = jf_multi(args);
	else if(!strcmp(f_name,"/"))		rc = jf_divi(args);
	else if(!strcmp(f_name,"modulo"))	rc = jf_modulo(args);
	else if(!strcmp(f_name,"cons"))		rc = jf_cons(args);
	else if(!strcmp(f_name,"append"))	rc = jf_append(args);
	else if(!strcmp(f_name,"car"))		rc = jf_car(args);
	else if(!strcmp(f_name,"cdr"))		rc = jf_cdr(args);
	else if(!strcmp(f_name,"list"))		rc = jf_list(args);
	else if(!strcmp(f_name,"eval"))		rc = js_eval(args);
	else if(!strcmp(f_name,"display"))	rc = jf_display(args);
	else if(!strcmp(f_name,"newline"))	rc = jf_newline(args);
	else if(!strcmp(f_name,"eq?"))		rc = jf_eq(args);
	else if(!strcmp(f_name,"equal?"))	rc = jf_equal(args);
	else if(!strcmp(f_name,"atom?"))	rc = jf_atom(args);
	else if(!strcmp(f_name,"nil?"))		rc = jf_null(args);
	else if(!strcmp(f_name,"null?"))	rc = jf_null(args);
	else if(!strcmp(f_name,"or"))		rc = jf_or(args);
	else if(!strcmp(f_name,"and"))		rc = jf_and(args);
	else if(!strcmp(f_name,"not"))		rc = jf_not(args);
	else if(!strcmp(f_name,"="))		rc = jf_cmp(args, CMP_EQUAL);
	else if(!strcmp(f_name,">"))		rc = jf_cmp(args, CMP_GREATER);
	else if(!strcmp(f_name,"<"))		rc = jf_cmp(args, CMP_LESS);
	else if(!strcmp(f_name,"<="))		rc = jf_cmp(args, CMP_LESS_EQUAL);
	else if(!strcmp(f_name,">="))		rc = jf_cmp(args, CMP_GREATER_EQUAL);
	else if(!strcmp(f_name,"list->string"))	rc = jf_list2string(args);

	sexp_free(func, 1);

	return rc;
}




int
main(int argc, char **argv)
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
	fclose(yyin);
	yylex_destroy();
	st_destory(global_table);
	return 0;
}
