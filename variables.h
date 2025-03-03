struct subprogram *make_subprogram(char *name, char **var_names, int var_names_end, struct node *cod);
void *insert_variable(void **var_head, const void *name);
void *find_variable(void *var_head, const void *name);
struct variable *varcpy(struct variable *dest, const struct variable *src);
void free_variable_list(struct variable *var_head);
void free_variable_map(struct variable **var_map);
void **variable_map_index(void **var_map, int index);
void *single_dereference_variable_map_func(void **var_map);
