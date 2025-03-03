#ifndef HELPERS_H
#define HELPERS_H

#include <parse.h>

#define T 50
#define MAP_CONSTANT 50
#define ERROR_LENGTH 200

/* function declarations
 * ---------------------
 */

/* alte functii ajutatoare */
void free_on_quit(void);
void print_lex(void);
void print_parsed(struct node *n);
void print_subprograme(void);

#endif

