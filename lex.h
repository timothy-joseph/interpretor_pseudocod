/* acestea sunt functii (si macrouri) ajutatoare, functia de lex principala este
 * declarata si definita in main.c odata cu toate variabilele globale
 */
#define TWOCHAR(c1, c2) \
	(cod[pos_in_string] == (c1) && \
	 cod[pos_in_string+1] && \
	 cod[pos_in_string+1] == (c2))
#define DIGITS "0123456789"
#define CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_"

void lex_verify_file_loop(char *filename);
struct files *lex_pop_from_file_stack(void);
char *lex_get_cod(char *filename, int ismain);
void insert_token(int type, char *value, int linia, int indent, char *filename); 
void lex_advance(char *cod, int *pos_in_string);
void lex_get_number_string(char *cod, int *pos_in_string, int linia, char *filename, char *ret);
void lex_get_string(char *cod, int *pos_in_string, char *ret);
void lex_get_string_in_quotes(char *cod, int *pos_in_string, int linia, char *filename, char *ret);
