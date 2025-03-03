#ifndef GLOBAL_VARS_DECLARED_H
#define GLOBAL_VARS_DECLARED_H

#include <constants.h>
#include <lex.h>
#include <parse.h>
#include <garbage.h>
#include <stack.h>

/* lex */
extern struct token *tokens;
extern struct files *files_head;
extern int tokens_size, tokens_end;
/* parser */
/* indent change pos este folosit si pentru altfel, repeta...cat timp
 * si repeta...pana cand
 */
extern int has_else, indent_change_pos, type_repeta;
extern struct node *parse_tree;
/* execution */
extern struct variable *global_var_map[MAP_CONSTANT];
extern struct garbage *possible_garbage_map[MAP_CONSTANT];
extern struct stack *stack_head;

#endif

