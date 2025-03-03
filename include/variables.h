#ifndef VARIABLES_H
#define VARIABLES_H

#include <constants.h>

#define TV_NUMAR TT_NUMAR
#define TV_CHAR TT_CHAR
#define TV_VECTOR TN_VECTOR
#define TV_TIP TN_TIP

union union_value {
	void *v;
	double d;
};

struct variable {
	char name[T];
	int type;
	int dimensiune;
	/* from_referinta este folosita pentru a nu elibera
	 * memoria unui vector obtinut din functia "referinta"
	 * in etapa de garbage collection
	 */
	int from_referinta;
	union union_value value;
	struct variable *next;
};

struct subprogram {
	char name[T];
	char var_names[T][T];
	int var_names_end;
	struct node *cod;
	struct subprogram *next;
};

struct subprogram *make_subprogram(char *name, char **var_names, int var_names_end, struct node *cod);
void *insert_variable(void **var_head, const void *name);
void *find_variable(void *var_head, const void *name);
struct variable *varcpy(struct variable *dest, const struct variable *src);
void free_variable_list(struct variable *var_head);
void free_variable_map(struct variable **var_map);
void **variable_map_index(void **var_map, int index);
void *single_dereference_variable_map_func(void **var_map);

extern struct map_utilities variable_map_utilities;

#endif

