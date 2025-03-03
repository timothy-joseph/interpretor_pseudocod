#ifndef LEX_H
#define LEX_H

#include <constants.h>

/* acestea sunt functii (si macrouri) ajutatoare, functia de lex principala este
 * declarata si definita in main.c odata cu toate variabilele globale
 */
#define TWOCHAR(c1, c2) \
	(cod[pos_in_string] == (c1) && \
	 cod[pos_in_string+1] && \
	 cod[pos_in_string+1] == (c2))
#define DIGITS "0123456789"
#define CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_"

enum {
	TT_ASSIGN,
	TT_SAU,
	TT_SI,
	TT_E,
	TT_NE,
	TT_L,
	TT_G,
	TT_LE,
	TT_GE,
	TT_PLUS,
	TT_MINUS,
	TT_MULTIPLY,
	TT_DIVIDE,
	TT_MOD,
	TT_EXPONENTIAL,
	TT_NOT,
	TT_LPAR,
	TT_RPAR,
	TT_LBRACKET,
	TT_RBRACKET,
	TT_VAR,
	TT_CHAR,
	TT_STRING,
	TT_NUMAR,
	TT_NEXT,
	TT_EOF,
	TT_DACA,
	TT_ALTFEL,
	TT_CATTIMP,
	TT_PANACAND,
	TT_PENTRU,
	TT_REPETA,
	TT_SUBPROGRAM,
	TT_RETURN,
	TT_BEGIN,
	TT_CITESTE,
	TT_CITESTETOT,
	TT_SCRIE,
	TT_SEPEXP,
	TT_TIP,
	TN_CALL,
	TN_REPETACATTIMP,
	TN_REPETAPANACAND,
	TN_VECTOR,
	TN_INDEXING,
	TV_FUNCTIE,
};

struct token {
	int type;
	char filename[T];
	int linia;
	int indent;
	char value[T];
};

struct files {
	/* lista de tip stack folosita
	 * pentru a tine cont de fisierele
	 * citite si de unde au fost referentiate
	 * in etapa de lex
	 */
	char filename[T];
	struct files *next;
};

void lex(char *filename, int ismain);
void lex_verify_file_loop(char *filename);
struct files *lex_pop_from_file_stack(void);
char *lex_get_cod(char *filename, int ismain);
void insert_token(int type, char *value, int linia, int indent, char *filename); 
void lex_advance(char *cod, int *pos_in_string);
void lex_get_number_string(char *cod, int *pos_in_string, int linia, char *filename, char *ret);
void lex_get_string(char *cod, int *pos_in_string, char *ret);
void lex_get_string_in_quotes(char *cod, int *pos_in_string, int linia, char *filename, char *ret);

#endif

