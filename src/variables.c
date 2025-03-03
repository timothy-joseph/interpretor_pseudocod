#include <stdlib.h>
#include <string.h>

#include <variables.h>
#include <map.h>
#include <errors.h>
#include <garbage.h>
#include <lex.h>
#include <parse.h>

struct map_utilities variable_map_utilities = {
	.hash_algo=adler32_string,
	.index_func=variable_map_index,
	.single_dereference_func=single_dereference_variable_map_func,
	.insert_func=insert_variable,
	.search_func=find_variable,
};

struct subprogram *
make_subprogram(char *name, char **var_names, int var_names_end, struct node *cod)
{
	/* face un pointer catre un subprogram - alocat dinamic */
	struct subprogram *tmp = (struct subprogram *)malloc(sizeof(struct subprogram));

	if (tmp == NULL)
		error(ERROR_MALLOC_SUBPROGRAM);
	strcpy(tmp->name, name);
	memcpy(tmp->var_names, var_names, T*T);
	tmp->var_names_end = var_names_end;
	tmp->cod = cod;
	return tmp;
}

void*
insert_variable(void **var_head, const void *name)
{
	/* insereaza o variabila si returneaza un pointer la ea */
	struct variable *tmp = (struct variable *)calloc(1, sizeof(struct variable));

	if (tmp == NULL)
		error(ERROR_MEMORIE_VARIABILA);
	strncpy(tmp->name, (const char *)name, T);
	tmp->next = *(struct variable **)var_head;
	*(struct variable **)var_head = tmp;
	return tmp;
}

void*
find_variable(void *var_head, const void *name)
{
	/* gaseste o variabila dupa nume in lista care incepe la var_head si
	 * o returneaza (in caz ca nu a gasit-o, returneaza NULL) */
	struct variable *tmp = NULL;

	for (tmp = (struct variable *)var_head; tmp; tmp = tmp->next)
		if (strcmp(tmp->name, (const char *)name) == 0)
			break;
	return tmp;
}

struct variable *
varcpy(struct variable *dest, const struct variable *src)
{
	/* aceasta functie nu copiaza si campurile de nume si de next, daca voiam sa
	 * copieze si campurile de nume si de next, foloseam memcpy
	 * nu le copiez, deoarece ar duce la pierderea variabile (or doar numele
	 * variabilei, ori doar campul de next, ori si campul de nume si campul
	 * de next)
	 */
	dest->type = src->type;
	dest->dimensiune = src->dimensiune;
	dest->value = src->value;
	return dest;
}

void
free_variable_list(struct variable *var_head)
{
	/* aceasta functie nu elibereaza si memoria alocata pentru TV_FUNCTIE si
	 * pentru TV_VECTOR * acest lucru se face cu possible_trash_collect
	 * (ceea ce elibereaza memoria conditionat)
	 */
	struct variable *tmp = NULL;

	while (var_head) {
		/* pop_stack este apelat la returnarea din functie, iar aceasta
		 * apeleaza free_variable_map care apeleaza aceasta funcie. acest
		 * pas este foarte important in garbage collection pentru ca
		 * scade numarul de referinte pe care le are un vector
		 */
		GARBAGE_COUNT_DECREASE_WRAPPER(*var_head);
		tmp = var_head;
		var_head = var_head->next;
		free(tmp);
	}
}

void
free_variable_map(struct variable **var_map)
{
	/* obs: var_map nu este alocata dinamic, ci doar elementele ei
	 * si nu elibereaza memoria alocata pentru TV_VECTOR sau TV_FUNCTIE
	 * acest lucru se face cu possible_garbage_collect
	 */
	int k;

	for (k = 0; k < MAP_CONSTANT; k++) {
		free_variable_list(var_map[k]);
		var_map[k] = NULL;
	}
}

void**
variable_map_index(void **var_map, int index)
{
	/* utilizat pentru map_utilities */
	return (void **)(&((struct variable **)var_map)[index % MAP_CONSTANT]);
}

void*
single_dereference_variable_map_func(void **var_map)
{
	/* utilizat pentru map_utilities */
	return (void *)(*(struct variable **)var_map);
}

