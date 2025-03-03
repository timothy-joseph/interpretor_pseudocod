#include <stdio.h>
#include <string.h>

#include <lex.h>
#include <parse.h>
#include <variables.h>
#include <execute.h>
#include <map.h>
#include <stack.h>
#include <garbage.h>
#include <global_vars_declared.h>
#include <helpers.h>

void
free_on_quit(void)
{
	while (lex_pop_from_file_stack());
	/* pop_stack elibereaza si mapa de variabile */
	while (stack_head)
		pop_stack();
	/* nu cred ca este necesar sa eliberez global_var_map separat */
	free_variable_map(global_var_map);
	free_parse_tree(parse_tree);
	free_garbage_map();
}

void
print_lex(void)
{
	int i;
	char temp[T];

	for (i = 0; i <= tokens_end; i++) {
		if (tokens[i].type == TT_PLUS) { strcpy(temp, "TT_PLUS"); }
		else if (tokens[i].type == TT_MINUS) { strcpy(temp, "TT_MINUS"); }
		else if (tokens[i].type == TT_MULTIPLY) { strcpy(temp, "TT_MULTIPLY"); }
		else if (tokens[i].type == TT_DIVIDE) { strcpy(temp, "TT_DIVIDE"); }
		else if (tokens[i].type == TT_MOD) { strcpy(temp, "TT_MOD"); }
		else if (tokens[i].type == TT_EXPONENTIAL) { strcpy(temp, "TT_EXPONENTIAL"); }
		else if (tokens[i].type == TT_ASSIGN) { strcpy(temp, "TT_ASSIGN"); }
		else if (tokens[i].type == TT_LE) { strcpy(temp, "TT_LE"); }
		else if (tokens[i].type == TT_GE) { strcpy(temp, "TT_GE"); }
		else if (tokens[i].type == TT_NE) { strcpy(temp, "TT_NE"); }
		else if (tokens[i].type == TT_L) { strcpy(temp, "TT_L"); }
		else if (tokens[i].type == TT_G) { strcpy(temp, "TT_G"); }
		else if (tokens[i].type == TT_E) { strcpy(temp, "TT_E"); }
		else if (tokens[i].type == TT_LPAR) { strcpy(temp, "TT_LPAR"); }
		else if (tokens[i].type == TT_RPAR) { strcpy(temp, "TT_RPAR"); }
		else if (tokens[i].type == TT_LBRACKET) { strcpy(temp, "TT_LBRACKET"); }
		else if (tokens[i].type == TT_RBRACKET) { strcpy(temp, "TT_RBRACKET"); }
		else if (tokens[i].type == TT_STRING) { strcpy(temp, "TT_STRING"); }
		else if (tokens[i].type == TT_CHAR) { strcpy(temp, "TT_CHAR"); }
		else if (tokens[i].type == TT_NUMAR) { strcpy(temp, "TT_NUMAR"); }
		else if (tokens[i].type == TT_NEXT) { strcpy(temp, "TT_NEXT"); }
		else if (tokens[i].type == TT_EOF) { strcpy(temp, "TT_EOF"); }
		else if (tokens[i].type == TT_VAR) { strcpy(temp, "TT_VAR"); }
		else if (tokens[i].type == TT_DACA) { strcpy(temp, "TT_DACA"); }
		else if (tokens[i].type == TT_ALTFEL) { strcpy(temp, "TT_ALTFEL"); }
		else if (tokens[i].type == TT_CATTIMP) { strcpy(temp, "TT_CATTIMP"); }
		else if (tokens[i].type == TT_PANACAND) { strcpy(temp, "TT_PANACAND"); }
		else if (tokens[i].type == TT_PENTRU) { strcpy(temp, "TT_PENTRU"); }
		else if (tokens[i].type == TT_REPETA) { strcpy(temp, "TT_REPETA"); }
		else if (tokens[i].type == TT_SUBPROGRAM) { strcpy(temp, "TT_SUBPROGRAM"); }
		else if (tokens[i].type == TT_RETURN) { strcpy(temp, "TT_RETURN"); }
		else if (tokens[i].type == TT_BEGIN) { strcpy(temp, "TT_BEGIN"); }
		else if (tokens[i].type == TT_SI) { strcpy(temp, "TT_SI"); }
		else if (tokens[i].type == TT_SAU) { strcpy(temp, "TT_SAU"); }
		else if (tokens[i].type == TT_NOT) { strcpy(temp, "TT_NOT"); }
		else if (tokens[i].type == TT_CITESTE) { strcpy(temp, "TT_CITESTE"); }
		else if (tokens[i].type == TT_SCRIE) { strcpy(temp, "TT_SCRIE"); }
		else if (tokens[i].type == TT_SEPEXP) { strcpy(temp, "TT_SEPEXP"); }
		printf("%d - %s:%s - line: %d indent: %d - %s\n", i, temp, tokens[i].value, tokens[i].linia, tokens[i].indent, tokens[i].filename);
	}
}

void
print_parsed(struct node *n)
{
	char temp[T];
	int i;

	if (n == NULL)
		return;
	if (n->type == TN_ASSIGN) strcpy(temp, "TN_ASSIGN");
	else if (n->type == TN_SAU) strcpy(temp, "TN_SAU");
	else if (n->type == TN_SI) strcpy(temp, "TN_SI");
	else if (n->type == TN_E) strcpy(temp, "TN_E");
	else if (n->type == TN_NE) strcpy(temp, "TN_NE");
	else if (n->type == TN_LE) strcpy(temp, "TN_LE");
	else if (n->type == TN_GE) strcpy(temp, "TN_GE");
	else if (n->type == TN_L) strcpy(temp, "TN_L");
	else if (n->type == TN_G) strcpy(temp, "TN_G");
	else if (n->type == TN_STRING) strcpy(temp, "TN_STRING");
	else if (n->type == TN_CHAR) strcpy(temp, "TN_CHAR");
	else if (n->type == TN_PLUS) strcpy(temp, "TN_PLUS");
	else if (n->type == TN_MINUS) strcpy(temp, "TN_MINUS");
	else if (n->type == TN_MULTIPLY) strcpy(temp, "TN_MULTIPLY");
	else if (n->type == TN_DIVIDE) strcpy(temp, "TN_DIVIDE");
	else if (n->type == TN_MOD) strcpy(temp, "TN_MOD");
	else if (n->type == TN_EXPONENTIAL) strcpy(temp, "TN_EXPONENTIAL");
	else if (n->type == TN_NOT) strcpy(temp, "TN_NOT");
	else if (n->type == TN_NUMAR) strcpy(temp, "TN_NUMAR");
	else if (n->type == TN_VAR) strcpy(temp, "TN_VAR");
	else if (n->type == TN_DACA) strcpy(temp, "TN_DACA");
	else if (n->type == TN_CATTIMP) strcpy(temp, "TN_CATTIMP");
	else if (n->type == TN_PANACAND) strcpy(temp, "TN_PANACAND");
	else if (n->type == TN_PENTRU) strcpy(temp, "TN_PENTRU");
	else if (n->type == TN_CITESTE) strcpy(temp, "TN_CITESTE");
	else if (n->type == TN_SCRIE) strcpy(temp, "TN_SCRIE");
	else if (n->type == TN_EOF) strcpy(temp, "TN_EOF");
	else if (n->type == TN_RETURN) strcpy(temp, "TN_RETURN");
	else if (n->type == TN_CALL) strcpy(temp, "TN_CALL");
	else if (n->type == TN_REPETACATTIMP) strcpy(temp, "TN_REPETACATTIMP");
	else if (n->type == TN_REPETAPANACAND) strcpy(temp, "TN_REPETAPANACAND");
	else if (n->type == TN_VECTOR) strcpy(temp, "TN_VECTOR");
	else if (n->type == TN_INDEXING) strcpy(temp, "TN_INDEXING");
	else if (n->type == TN_TIP) strcpy(temp, "TN_TIP");
	if (n->expresii[0])
		printf("(");
	printf("%s", temp);
	if (n->value[0] != 0)
		printf(":%s", n->value);

	if (n->type == TN_DACA) {
		for (i = 0; i < 3 && n->expresii[i] && i < T; i++) {
			printf(")expr%d:(", i);
			print_parsed(n->expresii[i]);
		}
	} else if (n->type == TN_PENTRU) {
		for (i = 0; i < 4 && i < T; i++) {
			printf(")expr%d:(", i);
			print_parsed(n->expresii[i]);
		}
	} else {
		for (i = 0; n->expresii[i] && i < T; i++) {
			printf(")expr%d:(", i);
			print_parsed(n->expresii[i]);
		}
	}

	if (n->expresii[0])
		printf(")");
	if (n->next)
		printf("\n");
	print_parsed(n->next);
}

void
print_subprograme(void)
{
	int k, i;
	struct variable *tmp;

	for (i = 0; i < MAP_CONSTANT; i++) for (tmp = global_var_map[i]; tmp; tmp = tmp->next) {
		if (tmp->type == TV_FUNCTIE) {
			struct subprogram *f = (struct subprogram *)tmp->value.v;
			printf("subprogramul %s(", f->name);
			for (k = 0; k < f->var_names_end; k++) {
				if (k == f->var_names_end-1)
					printf("%s", f->var_names[k]);
				else
					printf("%s, ", f->var_names[k]);
			}
			printf(") face\n");
			print_parsed(f->cod);
			printf("\n\n");
		}
	}
}

