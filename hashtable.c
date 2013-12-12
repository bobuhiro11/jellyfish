#include "common.h"

/*
 * Murmurhash2 By Austin Appleby
 * https://sites.google.com/site/murmurhash/
 */
static uint32_t MurmurHash2(const void * key, int len, uint32_t seed)
{

	uint32_t m = 0x5bd1e995;
	uint32_t r = 24;

	uint32_t h = seed ^ len;
	uint8_t * data = (uint8_t *)key;

	while(len >= 4){
		uint32_t k = *(uint32_t *)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch(len){
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

/*
 * insert data to hash table.
 * return data if success, NULL otherwise.
 */
struct s_exp *ht_insert(struct hashtable *table, const char *key, struct s_exp *data)
{
	int len = strlen(key);
	uint32_t h = MurmurHash2( (void*)key, len+1, (uint32_t)table);

	int n;
	for(n = 0;n<HASHTABLE_SIZE;n++){
		uint32_t i = (h + n) % HASHTABLE_SIZE;
		if(table[i].key[0] == '\0'){
			strncpy(table[i].key, key, KEYWORD_BUFLEN-1);
			table[i].data = data;
			return table[i].data;
		}else if (!strncmp(table[i].key, key, KEYWORD_BUFLEN-1)){
			table[i].data = data;
			return table[i].data;
		}
	}
	return 0;
}

/*
 * find data from hash table.
 * return data if success, sexp_undef otherwise.
 */
struct s_exp *ht_find(const struct hashtable *table, const char *key)
{
	int len = strlen(key);
	uint32_t h = MurmurHash2( (void*)key, len+1, (uint32_t)table);

	int n;
	for(n=0;n<HASHTABLE_SIZE;n++){
		int i = (h + n) % HASHTABLE_SIZE;
		if(table[i].key[0] == '\0')
			return sexp_undef;
		else if(strncmp(table[i].key, key, KEYWORD_BUFLEN)==0)
			return table[i].data;
	}
	return sexp_undef;
}

/*
 * dump hash table.
 */
void ht_dump(const struct hashtable *table)
{
	int i;
	for(i=0;i<HASHTABLE_SIZE;i++)
		if(table[i].key[0] != '\0'){
			printf("%-8s = ", table[i].key);
			write_sexp(table[i].data);
			printf("\n");
		}
}

/*
 * create hash table.
 * return table if success, NULL otherwise.
 */
struct hashtable* ht_create()
{
	struct hashtable *table;
	int i;

	if(!(table = (struct hashtable*)malloc(sizeof(struct hashtable) * HASHTABLE_SIZE)))
		return NULL;

	memset(table, 0, HASHTABLE_SIZE*sizeof(struct hashtable));

	for(i=0;i<HASHTABLE_SIZE;i++)
		table[i].data = nil;
	return table;
}

/*
 * destory hash table.
 */
void ht_destory(struct hashtable *table)
{
	free(table);
}
