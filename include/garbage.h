#ifndef GARBAGE_H
#define GARBAGE_H

#include <variables.h>

#define GARBAGE_COUNTING_CONDITION(X) \
	(!(X).from_referinta && ((X).type == TV_VECTOR || (X).type == TV_FUNCTIE))

/* Urmatoarele doua macro-uri verifica daca x.value.v este valid
 * pentru numararea de referinte si daca este, apeleaza functiile
 * recursive pentru modificarea numarului din garbage map
 */
#define GARBAGE_COUNT_INCREASE_WRAPPER(X)                                                  \
	do {                                                                               \
		if (GARBAGE_COUNTING_CONDITION((X)))                                       \
			increase_garbage_recursive((struct variable *)(X).value.v,         \
			                           (X).type == TV_VECTOR, (X).dimensiune); \
	} while (0);                                                                       \

#define GARBAGE_COUNT_DECREASE_WRAPPER(X)                                                  \
	do {                                                                               \
		if (GARBAGE_COUNTING_CONDITION((X)))                                       \
			decrease_garbage_recursive((struct variable *)(X).value.v,         \
			                           (X).type == TV_VECTOR, (X).dimensiune); \
	} while (0);                                                                       \


struct garbage {
	void *item;
	int count;
	struct garbage *next;
};

void *insert_possible_garbage(void **garbage_map, const void *to_insert);
void *garbage_search(void *garbage_head, const void *item);
void increase_garbage_recursive(struct variable *p, const int is_vector, const int dimensiune);
int decrease_garbage_recursive(struct variable *p, const int is_vector, const int dimensiune);
void garbage_collect(void);
void free_garbage_list(struct garbage *garbage_head);
void free_garbage_map(void);
void **garbage_index(void **garbage_map, int index);
void *garbage_single_dereferece(void **garbage_map);

extern struct map_utilities garbage_map_utilities;

#endif
