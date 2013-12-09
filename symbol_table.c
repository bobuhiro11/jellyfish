#include "common.h"

struct symbol_table *st_create(struct symbol_table *next){
	struct symbol_table *t = malloc(sizeof(struct symbol_table));
	if(!t)
		return NULL;
	t->next = next;
	t->table = ht_create();
	return t;
}

struct symbol_table *st_destory(struct symbol_table *stable){
	ht_destory(stable->table);
	free(stable);
}


void st_dump(const struct symbol_table *stable){
	printf("**symbol table**\n");
	ht_dump(stable->table);
}

struct s_exp *st_insert(struct symbol_table *stable, const char *key, struct s_exp *data){
	return ht_insert(stable->table, key, data);
}

struct s_exp *st_find(const struct symbol_table *stable, const char *key){
	return ht_find(stable->table, key);
}

void st_init(struct symbol_table *stable){
	st_insert(stable, "quote", special2sexp("quote"));
	st_insert(stable, "if", special2sexp("if"));
	st_insert(stable, "define", special2sexp("define"));

	st_insert(stable, "+", builtin2sexp("+"));
	st_insert(stable, "-", builtin2sexp("-"));
	st_insert(stable, "*", builtin2sexp("*"));
	st_insert(stable, "/", builtin2sexp("/"));

	st_insert(stable, "cons", builtin2sexp("cons"));
	st_insert(stable, "car", builtin2sexp("car"));
	st_insert(stable, "cdr", builtin2sexp("cdr"));
	st_insert(stable, "list", builtin2sexp("list"));
	st_insert(stable, "eval", builtin2sexp("eval"));

	st_insert(stable, "eq?", builtin2sexp("eq?"));
	st_insert(stable, "atom?", builtin2sexp("atom?"));
	st_insert(stable, "nil?", builtin2sexp("nil?"));

	st_insert(stable, "or", builtin2sexp("or"));
	st_insert(stable, "and", builtin2sexp("and"));
	st_insert(stable, "not", builtin2sexp("not"));
	st_insert(stable, "=", builtin2sexp("="));
	st_insert(stable, ">", builtin2sexp(">"));
	st_insert(stable, "<", builtin2sexp("<"));
	st_insert(stable, "<=", builtin2sexp("<="));
	st_insert(stable, ">=", builtin2sexp(">="));
}
