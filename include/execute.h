#ifndef EXECUTE_H
#define EXECUTE_H

#include <parse.h>
#include <variables.h>

struct variable execute(struct node *root);
struct variable execute_daca(struct node *root);
struct variable execute_while(struct node *root, int p);
struct variable execute_pentru(struct node *root, int p);
struct variable execute_citestetot(struct node *root, struct variable **var_map);
struct variable execute_citeste(struct node *root, struct variable **var_map);
struct variable execute_scrie(struct node *root, struct variable **var_map);
struct variable execute_call(struct node *root, struct variable **var_map, int p);
struct variable execute_operators(struct node *root, struct variable **var_map);
struct variable execute_assign(struct node *root, struct variable **var_map);
struct variable execute_vector(struct node *root);
struct variable *tn_var_get(struct node *root, struct variable **var_map);
struct variable *tn_var_get_or_make(struct node *root, struct variable **var_map);
struct variable *tn_indexing_helper(struct node *root, struct variable **var_map);

#endif

