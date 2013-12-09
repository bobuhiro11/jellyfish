#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"                 /* for token name generated by parser.y */

#define YY_SKIP_YYWRAP
#define nil  	((struct s_exp*)0)		/* nil type */
#define sexp_t	((struct s_exp*)0xFFFFFFFF)	/* true type */
#define sexp_f	((struct s_exp*)0xFFFFFFFe)	/* false type */

#define HASHTABLE_SIZE 	101
#define KEYWORD_BUFLEN 	256

/***************************************************
 * structure definition
 ***************************************************/
struct pair{
	struct s_exp *car;
	struct s_exp *cdr;
};

#define S_EXP_PAIR 	1
#define S_EXP_INTEGER	2
#define S_EXP_CHARACTER	3
#define S_EXP_SYMBOL	4
#define S_EXP_BUILTIN	5
#define S_EXP_SPECIAL	6
#define S_EXP_CLOJURE	7

struct s_exp{                   
	int ref;
	int type;
	union{
		struct pair pair;
		int integer;
		char character;
		char *symbol;
		char *builtin;
		char *special;
	}u;
};

struct hashtable{
	char   key[KEYWORD_BUFLEN];
	struct s_exp *data;
};

struct symbol_table{
	struct hashtable *table;
	struct symbol_table *next;
};

/***************************************************
 * function prototype definition
 ***************************************************/
int yylex(void);
void yyerror(char* s);
void prompt();
void prompt_multi();
int yywrap();
void write_sexp(struct s_exp *e);

struct s_exp *sexp_alloc();
struct s_exp *sexp_ref(struct s_exp *e);
struct s_exp *sexp_copy(struct s_exp *e);
void sexp_free(struct s_exp *e);

struct s_exp *integer2sexp(int i);
struct s_exp *character2sexp(char c);
struct s_exp *symbol2sexp(char *s);
struct s_exp *builtin2sexp(char *s);
struct s_exp *special2sexp(char *s);
struct s_exp *clojure2sexp(struct s_exp *e);
struct s_exp *cons(struct s_exp *car, struct s_exp *cdr);
struct s_exp *eval(struct s_exp *e);
struct s_exp *apply(struct s_exp *func, struct s_exp *args);
struct s_exp *apply_clojure_call(struct s_exp *clojure, struct s_exp *args);

struct symbol_table *st_create(struct symbol_table *next);
struct symbol_table *st_destory(struct symbol_table *stable);
struct s_exp *st_insert(struct symbol_table *stable, const char *key, struct s_exp *data);
struct s_exp *st_find(const struct symbol_table *stable, const char *key);
void st_dump(const struct symbol_table *stable);

struct hashtable* ht_create();
void ht_destory(struct hashtable *table);
struct s_exp *ht_insert(struct hashtable *table, const char *key, struct s_exp *data);
struct s_exp *ht_find(const struct hashtable *table, const char *key);
void ht_dump(const struct hashtable *table);

/***************************************************
 * external variable definition
 ***************************************************/
extern char *yytext;		/* lexer input */
extern FILE *yyin;		/* input file */
extern int interactive;		/* true if interactive mode, false otherwise */
extern int linenum;		/* line number */

#endif
