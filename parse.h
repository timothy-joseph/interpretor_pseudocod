/* functii ajutatoare parse */
struct node *parse_daca(int start, int indent, int tt_next_pos);
struct node *parse_while(int start, int indent, int tt_next_pos);
struct node *parse_repeta(int start, int indent, int tt_next_pos);
struct node *parse_pentru(int start, int indent, int tt_next_pos);
struct node *parse_citeste(int start, int indent, int tt_next_pos);
struct node *parse_subprogram(int start, int indent, int tt_next_pos);
struct node *parse_scrie(int start, int indent, int tt_next_pos);
/* functii ajutatoare parse_expression */
struct node *parse_data(int pos);
struct node *parse_vector(int start, int end);
struct node *parse_call(int start, int end);
struct node *parse_indexing(int start, int end);
/* alte tipuri de functii ajuatatoare pentru etapa de parse */
struct node *new_node(int type, char *value, int linia, char *filename);
int is_balanced_expression(int start, int end);
int is_all_in_paranthesis_expression(int start, int end);
int is_vector(int start, int end);
int get_pres_operator(struct token tmp);
void free_parse_tree(struct node *root);
