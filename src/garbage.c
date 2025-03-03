#include <stdlib.h>

#include <garbage.h>
#include <variables.h>
#include <map.h>
#include <errors.h>
#include <global_vars_declared.h>

struct map_utilities garbage_map_utilities = {
	.hash_algo=adler32_voidp,
	.index_func=garbage_index,
	.single_dereference_func=garbage_single_dereferece,
	.insert_func=insert_possible_garbage,
	.search_func=garbage_search,
};

void *
insert_possible_garbage(void **garbage_head, const void *to_insert)
{
	/* insereaza un element in possible_garbage_map */
	struct garbage *tmp = (struct garbage *)calloc(1, sizeof(struct garbage));

	if (tmp == NULL)
		error("error alloc failed for garbage collection");
	tmp->item = (void *)to_insert;
	tmp->count = 0;
	tmp->next = *(struct garbage **)garbage_head;
	*(struct garbage **)garbage_head = tmp;
	return tmp;
}


void*
garbage_search(void *garbage_head, const void *item)
{
	/* gaseste un element dupa campul item in list lantuita garbage_head */
	struct garbage *tmp = NULL;

	for (tmp = (struct garbage *)garbage_head; tmp; tmp = tmp->next)
		if (tmp->item == item)
			break;
	return tmp;
}

void
increase_garbage_recursive(struct variable *p, const int is_vector, const int dimensiune)
{
	/* creste numarul de referinte pe care il are p, NU numarul
	 * de referinte pe care il are p->value.v
	 *
	 * in cazul in care avem ceva de genul:
	 * x <- vector(3);
	 * x[1] <- vector(2);
	 * y <- x
	 * atunci x are 2 referinte, dar si x[1] are 2 referinte
	 * iar daca ultima instructiune era
	 * y <- x[1]
	 * atunci x are 1 referinta si x[1] are 2 referinte
	 */
	int i;
	struct garbage *tmp = insert_or_get_by_key((void **)possible_garbage_map, (void *)p,
	                                           sizeof((void *)p), garbage_map_utilities);
	if (!tmp)
		return;
	tmp->count++;

	if (is_vector) {
		for (i = 0; i < dimensiune; i++) {
			struct variable *tmp2 = &p[i];
			if (GARBAGE_COUNTING_CONDITION(*tmp2))
				increase_garbage_recursive(tmp2->value.v, tmp2->type == TV_VECTOR,
				                           tmp2->dimensiune);
		}
	}
}

int
decrease_garbage_recursive(struct variable *p, const int is_vector, const int dimensiune)
{
	/* scade numarul de referinte pe care il are p, NU numarul
	 * de referinte pe care il are p->value.v
	 *
	 * functia returneaza daca garbage_collect a fost rulat
	 *
	 * in cazul in care avem ceva de genul:
	 * x <- vector(3);
	 * x[1] <- vector(2);
	 * y <- x
	 * atunci x are 2 referinte, dar si x[1] are 2 referinte
	 * iar daca ultima instructiune era
	 * y <- x[1]
	 * atunci x are 1 referinta si x[1] are 2 referinte
	 */
	int i, to_collect = 0, col = 0;
	struct garbage *tmp = find_by_key((void **)possible_garbage_map, (void *)p, sizeof((void *)p),
	                  garbage_map_utilities);
	if (!tmp)
		return 0;
	tmp->count--;
	to_collect = tmp->count == 0;

	if (is_vector) {
		for (i = 0; i < dimensiune; i++) {
			struct variable *tmp2 = &p[i];
			if (GARBAGE_COUNTING_CONDITION(*tmp2))
				col = col || decrease_garbage_recursive(tmp2->value.v,
				                                        tmp2->type == TV_VECTOR,
				                                        tmp2->dimensiune);
		}
	}

	/* aici tmp s-ar putea sa fi fost deja elibarat
	 * nu mai este sigur sa fie accesat
	 */
	if (to_collect && !col) {
		/*
		int hash = garbage_map_utilities.hash_algo((void *)p, sizeof((void *)p));
		struct garbage *tmp3 = NULL, *prev = NULL;

		for (tmp3 = possible_garbage_map[hash]; tmp3;) {
			if (tmp3->item == (void *)p) {
				printf("collectat: %p\n", tmp->item);
				free(tmp3->item);

				if (tmp3 == possible_garbage_map[hash]) {
					possible_garbage_map[hash] = tmp->next;
					free(tmp3);
					tmp3 = possible_garbage_map[hash];
				} else {
					prev->next = tmp->next;
					free(tmp);
					tmp = prev->next;
				}
			} else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
		*/
		garbage_collect();
		return 1;
	}
	return col;
}

void
garbage_collect(void)
{
	int i;
	struct garbage *tmp = NULL, *prev;

	for (i = 0; i < MAP_CONSTANT; i++) {
		prev = NULL;
		for (tmp = possible_garbage_map[i]; tmp;) {
			if (tmp->count == 0) {
				free(tmp->item);
				if (tmp == possible_garbage_map[i]) {
					possible_garbage_map[i] = tmp->next;
					free(tmp);
					tmp = possible_garbage_map[i];
				} else {
					prev->next = tmp->next;
					free(tmp);
					tmp = prev->next;
				}
			} else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
	}
}

void
free_garbage_list(struct garbage *garbage_head)
{
	/* folosit la sfarsitul programului pentru a elibera memoria alocata
	 * pentru elementele din garbage_map
	 */
	struct garbage *tmp = NULL;

	while (garbage_head) {
		tmp = garbage_head;
		garbage_head = garbage_head->next;
		free(tmp);
	}
}

void
free_garbage_map(void)
{
	/* folosit la sfarsitul programului pentru a elibera memoria alocata
	 * pentru elementele din garbage_map
	 * note: garbage_map nu este alocata dinamic, ci doar elementele ei
	 */
	int k;

	for (k = 0; k < MAP_CONSTANT; k++)
		free_garbage_list(possible_garbage_map[k]);
}

void**
garbage_index(void **garbage_map, int index)
{
	/* folosit pentru map utilities */
	return (void **)(&((struct garbage **)garbage_map)[index % MAP_CONSTANT]);
}

void*
garbage_single_dereferece(void **garbage_map)
{
	/* folosit pentru map utilities */
	return (void *)(*(struct garbage **)garbage_map);
}

