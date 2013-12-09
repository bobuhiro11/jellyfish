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
