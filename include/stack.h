#ifndef STACK_H
#define STACK_H

struct stack {
	/* stackul folosit in executare pentru
	 * a tine cont de apeluri de subprograme
	 */
	int p;
	struct variable **var_map;
	struct stack *next;
};

/* instructiunile de stack folosite in executare */
struct stack *insert_stack(struct variable **var_map);
struct stack *pop_stack(void);

#endif

