/* Proiect de atestat de catre Cicu Timotei-Iosif
 * un interpretor de limbaj de programare are trei multe etape:
 * 1. lexarea -> transforma textul (codul) scris de utilizator in tokenuri
 *               si le pune intr-o lista
 * 2. parsarea -> transforma tokenurile de la pasul anterior intr-un parse_tree.
 *                parse_tree-ul este un arbore ce contine pasii de executare
 * 3. executarea -> executa parse_tree-ul
 *
 * limbajul de programare implementat de mine are si o a patra etapa numita
 * grabage collection. implementarea mea de garbage collection functioneaza
 * astfel:
 * -> atunci cand se alocheaza memorie, aceasa este adaugata unui hash_map
 *    care tine minte numarul de aparitii (este initializat cu 0)
 * -> atunci cand o variabila primeste o valoare de vector sau functie
 *    (acestea sunt alocate dinamic, cu exceptia vectorilor proveniti
 *    din functia referinta(x)), numarul de aparitii  a acestei adrese
 *    este incrementat cu 1
 * -> atunci cand un vector sau o functie sunt date ca parametru la apelul
 *    unei functii, numarul de aparitii este incrementat
 * -> atunci cand o variabila care era vector sau functie isi schimba valoarea
 *    numarul de aparitii este decrementat cu 1
 * -> atunci cand o functie returneaza sau se termina de executat, se verifica
 *    prin variabilele locale ale acelei functii daca exista vectori sau functii,
 *    caz in care, numarul de aparitii a adreselor respective de memorii este
 *    decrementat
 * -> daca numarul de aparitii a unei adrese a ajuns pana la 0, atunci aceasta este
 *    eliberata
 *
 * toate functiile specializate pentru o anumita etapa (lex, parse, execute) au prefixul care
 * indica de unde apartin si de asemenea sunt declarate si definite in fisierele lex.h, lex.c
 * parse.h, parse.c, execute.h, execute.c cu exceptia functiilor de stack si de garbage collection,
 * chiar daca aceste doua tipuri de functii sunt folosite doar in etapa de executare
 *
 * pentru mai multe detalii despre implementare si utilizare, cititi documente/pdf/implemetare.pdf
 * si comentariile functiilor individuale
 *
 * TODO: break parse_expression into smaller functions (45 lines max)
 */

/* includes 
 * --------
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "errors.h"

/* macros
 * ------
 */
#define T 50
#define MAP_CONSTANT 50
#define ERROR_LENGTH 200

#define SWITCH_ESCAPE_CHARACTER(CHR, STR, K)         \
	do {                                         \
		switch ((CHR)) {                     \
		case 'a': (STR)[(K)] = '\a'; break;  \
		case 'b': (STR)[(K)] = '\b'; break;  \
		case 'f': (STR)[(K)] = '\f'; break;  \
		case 'n': (STR)[(K)] = '\n'; break;  \
		case 't': (STR)[(K)] = '\t'; break;  \
		case 'r': (STR)[(K)] = '\r'; break;  \
		case 'v': (STR)[(K)] = '\v'; break;  \
		case '\\': (STR)[(K)] = '\\'; break; \
		case '\'': (STR)[(K)] = '\''; break; \
		case '\"': (STR)[(K)] = '\"'; break; \
		case '?': /* fallthrough */          \
		default: (STR)[(K)] = '?'; break;    \
		}                                    \
	} while (0)

/* aceste doua macro au nevoie de variabile definite in
 * interiorul functiei de unde sunt "apelate":
 * struct variable x = {0}, y = {0};
 * struct node *root;
 * root find un parametru al acelor functii
 */
#define BINARY_OPERATOR_LOGIC(OPERATOR)                             \
	do {                                                        \
		x = execute(root->expresii[0]);                     \
		y = execute(root->expresii[1]);                     \
		if ((x.type == TV_NUMAR || x.type == TV_CHAR) &&    \
		    (y.type == TV_NUMAR || y.type == TV_CHAR))      \
			ret.value.d = x.value.d OPERATOR y.value.d; \
		else if (x.type == TV_NUMAR || x.type == TV_CHAR)   \
			ret.value.d = x.value.d OPERATOR y.value.v; \
		else if (y.type == TV_NUMAR || y.type == TV_CHAR)   \
			ret.value.d = x.value.v OPERATOR y.value.d; \
		else                                                \
			ret.value.d = x.value.v OPERATOR y.value.v; \
		ret.type = TV_NUMAR;                                \
	} while (0)

#define BINARY_OPERATOR_NUMERIC(OPERATOR)                                \
	do {                                                             \
		x = execute(root->expresii[0]);                          \
		y = execute(root->expresii[1]);                          \
		if ((x.type == TV_NUMAR || x.type == TV_CHAR) ||         \
		    (y.type == TV_NUMAR || y.type == TV_NUMAR) ||        \
		    (y.type == TV_TIP || y.type == TV_TIP))              \
			ret.value.d = x.value.d OPERATOR y.value.d;      \
		else                                                     \
			error(ERROR_NOT_NUMAR, root->type, root->linia,  \
			      root->filename);                           \
		ret.type = TV_NUMAR;                                     \
	} while (0)

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

/* redefinesc macrou pentru tip nod cu cele de tip token,
 * fie si tipuri de nod
 */
#define TN_ASSIGN TT_ASSIGN
#define TN_SAU TT_SAU
#define TN_SI TT_SI
#define TN_E TT_E
#define TN_NE TT_NE
#define TN_L TT_L
#define TN_G TT_G
#define TN_LE TT_LE
#define TN_GE TT_GE
#define TN_PLUS TT_PLUS
#define TN_MINUS TT_MINUS
#define TN_MULTIPLY TT_MULTIPLY
#define TN_DIVIDE TT_DIVIDE
#define TN_MOD TT_MOD
#define TN_EXPONENTIAL TT_EXPONENTIAL
#define TN_NOT TT_NOT
#define TN_VAR TT_VAR
#define TN_CHAR TT_CHAR
#define TN_STRING TT_STRING
#define TN_NUMAR TT_NUMAR
#define TN_DACA TT_DACA
#define TN_CATTIMP TT_CATTIMP
#define TN_PANACAND TT_PANACAND
#define TN_PENTRU TT_PENTRU
#define TN_CITESTE TT_CITESTE
#define TN_CITESTETOT TT_CITESTETOT
#define TN_SCRIE TT_SCRIE
#define TN_TIP TT_TIP
#define TN_EOF TT_EOF
#define TN_RETURN TT_RETURN

#define TV_NUMAR TT_NUMAR
#define TV_CHAR TT_CHAR
#define TV_VECTOR TN_VECTOR
#define TV_TIP TN_TIP

/* types
 * -----
 */
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

struct node {
	/* campul value poate fi: numele variabilei/functiei,
	 *                        valoarea numarului,
	 *                        tipul tipului
	 */
	char value[T];
	int type;
	char filename[T];
	int linia;
	struct node *expresii[T];
	struct node *next;
};

union union_value {
	void *v;
	double d;
};

struct variable {
	char name[T];
	int type;
	int dimensiune;
	/* from_referinta este folosita pentru a nu elibera
	 * memoria unui vector obtinut din functia "referinta"
	 * in etapa de garbage collection
	 */
	int from_referinta;
	union union_value value;
	struct variable *next;
};

struct subprogram {
	char name[T];
	char var_names[T][T];
	int var_names_end;
	struct node *cod;
	struct subprogram *next;
};

struct stack {
	/* stackul folosit in executare pentru
	 * a tine cont de apeluri de subprograme
	 */
	int p;
	struct variable **var_map;
	struct stack *next;
};

struct garbage {
	void *item;
	int count;
	struct garbage *next;
};

struct map_utilities {
	unsigned int (*hash_algo)(const void *, size_t);
	void **(*index_func)(void **, int);
	void *(*single_dereference_func)(void **);
	void *(*insert_func)(void **, const void *);
	void *(*search_func)(void *, const void *);
};

/* function declarations
 * ---------------------
 */
static void lex(char *filename, int ismain);
static struct node *parse(int start, int indent);
static struct node *parse_expression(int start, int end);
static struct variable execute(struct node *root);

/* global variables
 * ----------------
 */
/* lex */
struct token *tokens;
struct files *files_head;
int tokens_size, tokens_end;
/* parser */
/* indent change pos este folosit si pentru altfel, repeta...cat timp
 * si repeta...pana cand
 */
int has_else, indent_change_pos, type_repeta;
struct node *parse_tree = NULL;
/* execution */
struct variable *global_var_map[MAP_CONSTANT] = {0};
struct garbage *possible_garbage_map[MAP_CONSTANT] = {0};
struct stack *stack_head = NULL;

#include "helpers.h"


/* function definitions
 * --------------------
 */
static void
lex(char *filename, int ismain)
{
	char *cod = NULL;
	int pos_in_string = 0, indent = 0, linia = 1;

	/* cod este alocat dinamic */
	lex_verify_file_loop(filename);
	cod = lex_get_cod(filename, ismain);

	while (cod[pos_in_string]) {
		if (strchr(" \t", cod[pos_in_string])) {
			lex_advance(cod, &pos_in_string);
		} else if (cod[pos_in_string] == '\\') {
			linia++;
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '#') {
			while (cod[pos_in_string] && cod[pos_in_string] != '\n' &&
			       cod[pos_in_string] != EOF)
				lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '+') {
			insert_token(TT_PLUS, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '-') {
			insert_token(TT_MINUS, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '*') {
			insert_token(TT_MULTIPLY, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '/') {
			insert_token(TT_DIVIDE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '%') {
			insert_token(TT_MOD, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '^') {
			insert_token(TT_EXPONENTIAL, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		/* este necesar ca <-, <=, >=, != sa fie deasupra <, >, si ! */
 		} else if (TWOCHAR('<', '-')) {
			insert_token(TT_ASSIGN, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (TWOCHAR('<', '=')) {
			insert_token(TT_LE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (TWOCHAR('>', '=')) {
			insert_token(TT_GE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (TWOCHAR('!', '=')) {
			insert_token(TT_NE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '<') {
			insert_token(TT_L, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '>') {
			insert_token(TT_G, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '=') {
			insert_token(TT_E, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '!') {
			insert_token(TT_NOT, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '(') {
			insert_token(TT_LPAR, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == ')') {
			insert_token(TT_RPAR, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '[') {
			insert_token(TT_LBRACKET, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == ']') {
			insert_token(TT_RBRACKET, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == ',') {
			insert_token(TT_SEPEXP, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '|') {
			indent++;
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == EOF) {
			insert_token(TT_EOF, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '\n' || cod[pos_in_string] == ';') { 
			if (tokens_end != -1 &&
			    tokens[tokens_end].type !=  TT_NEXT)
				insert_token(TT_NEXT, "", linia, indent, filename);
			if (cod[pos_in_string] == '\n') {
				linia++;
				indent = 0;
			}
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '\'') {
			int k = 0;
			char next_char = 0, next_next_char = 0, temp[2] = "";

			lex_advance(cod, &pos_in_string);
			next_char = cod[pos_in_string];
			lex_advance(cod, &pos_in_string);
			next_next_char = cod[pos_in_string];

			if (next_char == '\\')
				lex_advance(cod, &pos_in_string);

			if ((next_char != '\\' && next_next_char != '\'') ||
			    (next_char == '\\' && cod[pos_in_string] != '\''))
				error(ERROR_CHAR_LEX_UNDEFINED_WELL, linia, filename);

			if (next_char == '\\')
				SWITCH_ESCAPE_CHARACTER(next_next_char, temp, 0);
			else
				temp[0] = next_char;

			temp[1] = 0;
			lex_advance(cod, &pos_in_string);
			insert_token(TT_CHAR, temp, linia, indent, filename);

		} else if (cod[pos_in_string] == '"') {
			char temp[T];

			lex_get_string_in_quotes(cod, &pos_in_string, linia,
			                         filename, temp);
			lex_advance(cod, &pos_in_string);
			insert_token(TT_STRING, temp, linia, indent,
			             filename);

		} else if (strchr(DIGITS, cod[pos_in_string])) {
			char temp[T];

			lex_get_number_string(cod, &pos_in_string, linia,
			                      filename, temp);
			insert_token(TT_NUMAR, temp, linia, indent, filename);

		} else if (strchr(CHARS, cod[pos_in_string])) {
			char temp[T];

			lex_get_string(cod, &pos_in_string, temp);
			if (strcmp(temp, "daca") == 0) {
				insert_token(TT_DACA, "", linia, indent, filename);
			} else if (strcmp(temp, "altfel") == 0) {
				insert_token(TT_ALTFEL, "", linia, indent, filename);
			} else if (strcmp(temp, "cat") == 0) {
				if (cod[pos_in_string]) {
					lex_advance(cod, &pos_in_string);
					lex_get_string(cod, &pos_in_string, temp);
					if (strcmp(temp, "timp") == 0)
						insert_token(TT_CATTIMP, "",
						             linia, indent, filename);
					else
						error(ERROR_CATTIMP_SYNTAX,
						      linia, filename);
				} else {
					error(ERROR_CATTIMP_SYNTAX, linia,
					      filename);
				}
			} else if (strcmp(temp, "pana") == 0) {
				if (cod[pos_in_string]) {
					lex_advance(cod, &pos_in_string);
					lex_get_string(cod, &pos_in_string, temp);
					if (strcmp(temp, "cand") == 0)
						insert_token(TT_PANACAND, "",
						             linia, indent,
							     filename);
					else
						error(ERROR_PANACAND_SYNTAX,
						      linia, filename);
				} else {
					error(ERROR_PANACAND_SYNTAX, linia,
					      filename);
				}
			} else if (strcmp(temp, "pentru") == 0) {
				insert_token(TT_PENTRU, "", linia, indent, filename);
			} else if (strcmp(temp, "repeta") == 0 ) {
				insert_token(TT_REPETA, "", linia, indent, filename);
			} else if (strcmp(temp, "subprogramul") == 0) {
				insert_token(TT_SUBPROGRAM, "", linia, indent,
				             filename);
			} else if (strcmp(temp, "returneaza") == 0 ||
			           strcmp(temp, "intoarce") == 0) {
				insert_token(TT_RETURN, "", linia, indent, filename);
			} else if (strcmp(temp, "atunci") == 0 ||
			           strcmp(temp, "executa") == 0 ||
			           strcmp(temp, "face") == 0) {
				insert_token(TT_BEGIN, "", linia, indent, filename);
			} else if (strcmp(temp, "si") == 0) {
				insert_token(TT_SI, "", linia, indent, filename);
			} else if (strcmp(temp, "sau") == 0) {
				insert_token(TT_SAU, "", linia, indent, filename);
			} else if (strcmp(temp, "not") == 0) {
				insert_token(TT_NOT, "", linia, indent, filename);
			} else if (strcmp(temp, "citeste") == 0) {
				insert_token(TT_CITESTE, "", linia, indent, filename);
			} else if (strcmp(temp, "citestetot") == 0) {
				insert_token(TT_CITESTETOT, "", linia, indent,
				             filename);
			} else if (strcmp(temp, "scrie") == 0) {
				insert_token(TT_SCRIE, "", linia, indent, filename);
			} else if (strcmp(temp, "TV_NUMAR") == 0) {
				insert_token(TT_TIP, "1", linia, indent, filename);
			} else if (strcmp(temp, "TV_CHAR") == 0) {
				insert_token(TT_TIP, "2", linia, indent, filename);
			} else if (strcmp(temp, "TV_VECTOR") == 0) {
				insert_token(TT_TIP, "3", linia, indent, filename);
			} else if (strcmp(temp, "TV_TIP") == 0) {
				insert_token(TT_TIP, "4", linia, indent, filename);
			} else if (strcmp(temp, "TV_ERROR") == 0) {
				insert_token(TT_TIP, "0", linia, indent, filename);
			} else if (strcmp(temp, "sursa") == 0) {
				char tmpfilename[T];

				/* compactare pana la " sau < */
				while (cod[pos_in_string] && cod[pos_in_string] != '"')
					lex_advance(cod, &pos_in_string);

				if (cod[pos_in_string] == '"')
					lex_get_string_in_quotes(cod, &pos_in_string,
					                         linia, filename,
								 tmpfilename);
				else
					error("sursa definita gresit la linia %d",
					      linia);
				lex_advance(cod, &pos_in_string);
				lex(tmpfilename, 0);
			} else {
				insert_token(TT_VAR, temp, linia, indent, filename);
			}
		} else {
			error(ERROR_UNKNOWN_CHAR_FORMAT, cod[pos_in_string],
			      linia, filename);
		}
	}

	free(cod);
	lex_pop_from_file_stack();
}

static struct node*
parse(int start, int indent)
{
	int k, tt_next_pos = start;
	struct node *ret = NULL;

	if (start > tokens_end) {
		return NULL;
	} else if (tokens[start].indent < indent) {
		indent_change_pos = start;
		return NULL;
	}

	for (k = start; k <= tokens_end; k++) {
		if (tokens[k].indent > indent)
			error(ERROR_INDENT_FORMAT, tokens[k].linia,
			      tokens[k].filename);
		if (tokens[k].type == TT_NEXT || tokens[k].type == TT_EOF) {
			tt_next_pos = k;
			break;
		}
	}

	switch (tokens[start].type) {
	case TT_DACA:
		return parse_daca(start, indent, tt_next_pos);
	case TT_ALTFEL:
		indent_change_pos = start;
		has_else = 1;
		return NULL;
	case TT_CATTIMP: /* FALLTHROUGH */
	case TT_PANACAND:
		return parse_while(start, indent, tt_next_pos);
	case TT_REPETA:
		return parse_repeta(start, indent, tt_next_pos);
	case TT_PENTRU:
		return parse_pentru(start, indent, tt_next_pos);
	case TT_SUBPROGRAM:
		return parse_subprogram(start, indent, tt_next_pos);
	case TT_CITESTETOT: /* FALLTHROUGH */
	case TT_CITESTE:
		return parse_citeste(start, indent, tt_next_pos);
	case TT_SCRIE:
		return parse_scrie(start, indent, tt_next_pos);
	case TT_RETURN:
		ret = new_node(TN_RETURN, "", tokens[start].linia,
		               tokens[start].filename);
		ret->expresii[0] = parse_expression(start + 1, tt_next_pos - 1);
		ret->next = parse(tt_next_pos + 1, indent);
		return ret;
	case TT_EOF:
		ret = new_node(TN_EOF, "", tokens[start].linia,
		              tokens[start].filename);
		return ret;
	default:
		ret = parse_expression(start, tt_next_pos - 1);
		ret->next = parse(tt_next_pos + 1, indent);
		return ret;
	}
}

static struct node*
parse_expression(int start, int end)
{
	/* aceasta functie creeaza o expresie cu tip de notare prefix
	 * si este pusa intr-un arbore
	 * notarea prefix nu are nevoie de paranteze si in notarea
	 * prefix, simbolului cu cel mai mic grad (cel care sa
	 * fie executat ultimul) este pus primul. de asemenea, daca
	 * apar mai multe simboluri cu acelasi grad minim si valide
	 * (sa nu fie in paranteze), se va pune pe primul loc in
	 * notarea prefix cel care apare ultimul in notarea infix
	 * (normala)
	 * ex: 3 + 4 => + 3 4
	 * ex: 1 + 2 * 4 => + 1 * 2 4
	 * ex: (1 + 2) * 4 => * + 1 2 4
	 * ex: 1 + 2 + 3 => ++123
	 * ex: 1 + (2 + 3) => +1+23
	 * ex: 0 - 1 - 5 => --015
	 * ex: 0 - (1 - 5) => -0-15
	 * ex: 0 - 1 + 5 => +-015

	 0 - 1 - 5 => -0-15
	 0 - 1 - 5 => --015
	 */
	int lowest_presidency_idx = -1, lowest_pres = ERROR_PRESIDENCY_NUMBER - 1,
	    paranthesis = 0, brackets = 0, k;
	struct token v;
	struct node *ret = NULL;

	if (start > end)
		return NULL;
	if (!is_balanced_expression(start, end))
		error(ERROR_PARANTHESIS_NOT_MATCHING_FORMAT,
		      tokens[start].linia, tokens[start].filename);
	if (start == end)
		return parse_data(start);
	if (is_vector(start, end))
		return parse_vector(start, end);
	if (is_all_in_paranthesis_expression(start, end))
		return parse_expression(start+1, end-1);

	/* aici se gaseste indexul simbolului cu cel mai
	 * mic grad
	 */
	paranthesis = brackets = 0;
	for (k = start; k <= end; k++) {
		v = tokens[k];

		if (v.type == TT_LPAR)
			paranthesis++;
		else if (v.type == TT_RPAR)
			paranthesis--;

		if (v.type == TT_LBRACKET) {
			brackets++;
		} else if (v.type == TT_RBRACKET) {
			brackets--;
			/* parantezele au fost verificate mai sus cu is_balanced_expression */
			if (brackets < 0)
				error(ERROR_EXPERSSION_MALFORMED_FORMAT, v.linia,
				      v.filename);
		}

		if (paranthesis == 0 && brackets == 0) {
			/* trebuie sa nu fie in parenteze rotunde sau patrate,
			 * deaorece ar modifica gradul
			 * parantezele patrate sunt folosite la indexing
			 */
			int cur_pres = get_pres_operator(v);

			if (cur_pres <= lowest_pres) {
				lowest_presidency_idx = k;
				lowest_pres = cur_pres;
			}
		}
	}
	/* parantezele au fost verificate mai sus cu is_balanced_expression */
	if (brackets) 
		error(ERROR_EXPERSSION_MALFORMED_FORMAT, v.linia, v.filename);

	if (lowest_presidency_idx == -1) {
		/* adica nu a fost gasit niciun operator */
		if (tokens[start].type == TT_VAR && tokens[start+1].type == TT_LPAR)
			return parse_call(start, end);
		else if (tokens[start].type == TT_VAR &&
		         tokens[start+1].type == TT_LBRACKET)
			return parse_indexing(start, end);
		else
			error(ERROR_EXPERSSION_MALFORMED_FORMAT,
			      tokens[start].linia, tokens[start].filename);

	} else if (tokens[lowest_presidency_idx].type == TT_NOT) {
		ret = new_node(TN_NOT, "", tokens[lowest_presidency_idx].linia,
		               tokens[lowest_presidency_idx].filename);
		ret->expresii[1] = parse_expression(lowest_presidency_idx+1, end);

	} else {
		ret = new_node(tokens[lowest_presidency_idx].type, "",
		               tokens[lowest_presidency_idx].linia,
			       tokens[lowest_presidency_idx].filename);
		ret->expresii[0] = parse_expression(start, lowest_presidency_idx-1);
		ret->expresii[1] = parse_expression(lowest_presidency_idx+1, end);

		/* in caz de se foloseste un numar mai mic ca zero,
		 * acesta va fi interpretat ca o operatie 0 - abs(numar)
		 * ex:
		 * x <- -1 ----> x <- 0 - 1 ----> <- x - 0 1 notare prefix
		 * x <- -1 + 0 ----> x <- 0 - 1 + 0 ----> +-010
		 * x <- -1 - 5 ----> x <- 0 - 1 - 5 ----> --015
		 */

		if (tokens[lowest_presidency_idx].type == TT_MINUS &&
		    !ret->expresii[0])
			ret->expresii[0] = new_node(TT_NUMAR, "0",
			                      tokens[lowest_presidency_idx].linia,
					      tokens[lowest_presidency_idx].filename);

		if (!ret->expresii[0] || !ret->expresii[1])
			error(ERROR_EXPERSSION_MALFORMED_FORMAT,
			      tokens[start].linia, tokens[start].filename);
	}
	return ret;
}

static struct variable
execute(struct node *root)
{
	struct variable **var_map = NULL, ret = {0}, y = {0}, *x = NULL;
	int p;

	if (root == NULL)
		return ret;
	if (stack_head == NULL)
		return ret;

	p = stack_head->p;
	var_map = stack_head->var_map;

	while (root) {
		switch (root->type) {
		case TN_DACA:
			ret = execute_daca(root);
			break;
		case TN_CATTIMP: /* FALLTHROUGH */
		case TN_PANACAND: /* FALLTHROUGH */
		case TN_REPETACATTIMP: /* FALLTHROUGH */
		case TN_REPETAPANACAND:
			ret = execute_while(root, p);
			break;
		case TN_PENTRU:
			/* cu verificare daca nu s-a returnat din subprogram */
			ret = execute_pentru(root, p);

			/* sau
			for (execute(root->expresii[0]), y = execute(root->expresii[1]);
			     p == stack_head->p && (y.value.d || y.value.v);
			     execute(root->expresii[2]), y = execute(root->expresii[1])) {
				ret = execute(root->expresii[3]);
				
				if (p != stack_head->p)
					break;
			}
			*/
			break;
		case TN_CITESTETOT:
			ret = execute_citestetot(root, var_map);
			break;
		case TN_CITESTE:
			ret = execute_citeste(root, var_map);
			break;
		case TN_SCRIE:
			ret = execute_scrie(root, var_map);
			break;
		case TN_CALL:
			ret = execute_call(root, var_map, p);
			break;
		case TN_RETURN:
			ret = execute(root->expresii[0]);
			pop_stack();
			return ret;
			break;
		case TN_EOF:
			free_on_quit();
			exit(0);
			break;
		case TN_ASSIGN: /* FALLTHROUGH */
		case TN_SAU: /* FALLTHROUGH */
		case TN_SI: /* FALLTHROUGH */
		case TN_E: /* FALLTHROUGH */
		case TN_NE: /* FALLTHROUGH */
		case TN_L: /* FALLTHROUGH */
		case TN_G: /* FALLTHROUGH */
		case TN_LE: /* FALLTHROUGH */
		case TN_GE: /* FALLTHROUGH */
		case TN_PLUS: /* FALLTHROUGH */
		case TN_MINUS: /* FALLTHROUGH */
		case TN_MULTIPLY: /* FALLTHROUGH */
		case TN_DIVIDE: /* FALLTHROUGH */
		case TN_MOD: /* FALLTHROUGH */
		case TN_EXPONENTIAL: /* FALLTHROUGH */
		case TN_NOT:
			ret = execute_operators(root, var_map);
			break;
		case TN_NUMAR:
			ret.type = TV_NUMAR;
			ret.value.d = atof(root->value);
			break;
		case TN_CHAR:
			ret.type = TV_CHAR;
			ret.value.d = *root->value;
			break;
        	case TN_VECTOR:
			ret = execute_vector(root);
			break;
		case TN_TIP:
			ret.type = TV_TIP;
			switch(*root->value - '0') {
			case 0: ret.value.d = 0; break;
			case 1: ret.value.d = TV_NUMAR; break;
			case 2: ret.value.d = TV_CHAR; break;
			case 3: ret.value.d = TV_VECTOR; break;
			case 4: ret.value.d = TV_TIP; break;
			default: ret.value.d = 0; break;
			}
			break;
		case TN_VAR:
			x = tn_var_get(root, var_map);
			varcpy(&ret, x);
			break;
		case TN_INDEXING:
			x = tn_indexing_helper(root, var_map);
			varcpy(&ret, x);
			break;
		}

		if (p == stack_head->p)
			root = root->next;
		else if (p > stack_head->p)
			root = NULL;
		else if (p < stack_head->p)
			/* s-ar putea sa nu ajunga niciodata in acest caz */
			while (p < stack_head->p)
				pop_stack();
	}
	return ret;
}

int
main(int argc, char *argv[])
{
	char filename[T];

	/* pentru testare */
	if (argc == 1)
		strcpy(filename, "test.pseudo");
	else
		strcpy(filename, argv[1]);
	
	/* alocare memorie token, lex si eliberare cod */
	tokens = (struct token *)calloc(T, sizeof(struct token));
	if (tokens == NULL)
		error(ERROR_ALLOC_TOKENS);
	tokens_size = T;
	tokens_end = -1;
	lex(filename, 1);
	/*print_lex();*/

	/* eliberare memorie files list */
	while (lex_pop_from_file_stack());

	/* parse si eliberare tokens */
	parse_tree = parse(0, 0);
	/*
	printf("\n\nSubprograme:\n\n");
	print_subprograme();
	printf("\nMain:\n\n");
	print_parsed(parse_tree);
	*/
	free(tokens);

	/* executare */
	/*printf("\n\nOutput la program:\n");*/
	insert_stack(global_var_map);
	execute(parse_tree);

	free_on_quit();

	return 0;
}
