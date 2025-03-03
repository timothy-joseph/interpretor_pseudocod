/* stackul folosit in executare pentru a tine cont
 * de subprogramele apelate
 */
#include <stdlib.h>

#include <variables.h>
#include <stack.h>
#include <errors.h>
#include <global_vars_declared.h>

struct stack*
insert_stack(struct variable **var_map)
{
	/* insereasa var_map in stack
	 * si un p incrementat ca un
	 * fel de id
	 */
	struct stack *tmp = (struct stack *)malloc(sizeof(struct stack));

	if (tmp == NULL)
		error(ERROR_STACK_INSERT);
	tmp->p = 0;
	if (stack_head)
		tmp->p = stack_head->p + 1;
	tmp->next = stack_head;
	tmp->var_map = var_map;
	stack_head = tmp;
	return tmp;
}

struct stack*
pop_stack(void)
{
	/* scoate din stack si elibereaza var_map scos (prin apelarea
	 * functiei free_variable_list)
	 */
	struct stack *tmp = NULL;

	if (!stack_head)
		return NULL;
	tmp = stack_head;
	free_variable_map(stack_head->var_map);
	stack_head = stack_head->next;
	free(tmp);
	return stack_head;
}

