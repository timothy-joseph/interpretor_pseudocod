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

#include <map.h>
#include <variables.h>
#include <stack.h>
#include <garbage.h>
#include <lex.h>
#include <parse.h>
#include <execute.h>
#include <errors.h>

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
