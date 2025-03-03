#ifndef PARSE_H
#define PARSE_H

#include <lex.h>

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

/* functii ajutatoare parse */
struct node *parse(int start, int indent);
struct node *parse_expression(int start, int end);
struct node *parse_daca(int start, int indent, int tt_next_pos);
struct node *parse_while(int start, int indent, int tt_next_pos);
struct node *parse_repeta(int start, int indent, int tt_next_pos);
struct node *parse_pentru(int start, int indent, int tt_next_pos);
struct node *parse_citeste(int start, int indent, int tt_next_pos);
struct node *parse_subprogram(int start, int indent, int tt_next_pos);
struct node *parse_scrie(int start, int indent, int tt_next_pos);
/* functii ajutatoare parse_expression */
struct node *parse_data(int pos);
struct node *parse_vector(int start, int end);
struct node *parse_call(int start, int end);
struct node *parse_indexing(int start, int end);
/* alte tipuri de functii ajuatatoare pentru etapa de parse */
struct node *new_node(int type, char *value, int linia, char *filename);
int is_balanced_expression(int start, int end);
int is_all_in_paranthesis_expression(int start, int end);
int is_vector(int start, int end);
int get_pres_operator(struct token tmp);
void free_parse_tree(struct node *root);

#endif

