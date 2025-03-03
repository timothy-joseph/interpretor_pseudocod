#include "lex.h"

void
lex_verify_file_loop(char *filename)
{
	struct files *tmp = files_head;

	while (tmp) {
		if (strcmp(tmp->filename, filename) == 0)
			error("s-a detectat un loop de fisiere in etapa de lex, fisierul este %s", filename);
		tmp = tmp->next;
	}
	
	tmp = (struct files *)calloc(1, sizeof(struct files));

	if (tmp == NULL)
		error("lex, alocare memorie pentru %s la verify file loop a esuta", filename);

	strncpy(tmp->filename, filename, T);
	tmp->next = files_head;
	files_head = tmp;
}

struct files*
lex_pop_from_file_stack(void)
{
	struct files *tmp = NULL;

	if (!files_head)
		return NULL;
	tmp = files_head;
	files_head = files_head->next;
	free(tmp);
	return files_head;
}

char*
lex_get_cod(char *filename, int ismain)
{
	/* gasire marime cod */
	char *cod = NULL;
	int fsize = 0;
	FILE *f = fopen(filename, "rb");

	if (f == NULL)
		error("fisierul %s nu poate fi citit", filename);

	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	/* alocare memorie cod si citire */
	cod = (char *)calloc(fsize+2, 1);

	if (cod == NULL)
		error("nu s-a putut aloca destula memorie pentru a retine codul din fisierul %s", filename);

	fread(cod, 1, fsize, f);
	fclose(f);

	if (ismain)
		cod[fsize] = EOF;
	cod[fsize+1] = 0;

	return cod;
}

void
insert_token(int type, char *value, int linia, int indent, char *filename)
{
	/* pune un token in lista de tokenuri */
	tokens_end++;
	if (tokens_end >= tokens_size) {
		tokens = realloc(tokens, tokens_size * sizeof(struct token) +
		                         T * sizeof(struct token));
		if (tokens == NULL)
			error(ERROR_REALLOC_TOKENS);
		tokens_size += T;
	}
	tokens[tokens_end].type = type;
	strncpy(tokens[tokens_end].value, value, T);
	tokens[tokens_end].linia = linia;
	strncpy(tokens[tokens_end].filename, filename, T);
	tokens[tokens_end].indent = indent;
}

void
lex_advance(char *cod, int *pos_in_string)
{
	/* avanseaza cu un caracter cand se citeste codul */
	if (cod[*pos_in_string] == 0)
		return;
	(*pos_in_string)++;
}

void
lex_get_number_string(char *cod, int *pos_in_string, int linia, char *filename, char *ret)
{
	/* ia un sir de cifre si il returneaza intr-un sir de
	 * caractere in variabila ret
	 */
	int dots = 0, k = 0;

	ret[0] = 0;
	while (cod[*pos_in_string] && strchr("0123456789.", cod[*pos_in_string]) &&
	       k < T-1) {
		if (cod[*pos_in_string] == '.')
			dots++;
		ret[k++] = cod[*pos_in_string];
		lex_advance(cod, pos_in_string);
	}

	if (dots > 1)
		error(ERROR_NUMAR_DOTS_FORMAT, linia, filename);
	if (k == T-1)
		error(ERROR_EXCEEDE_T, "lex number string", T-1, 0, "");

	ret[k] = 0;
}

void
lex_get_string(char *cod, int *pos_in_string, char *ret)
{
       	/* ia un sir de caractere de la caracterul curent pana
	 * la spatiu (nu pentru TT_STRING)
	 */
	int k = 0;

	ret[0] = 0;
	while (cod[*pos_in_string] && strchr(CHARS, cod[*pos_in_string]) &&
	       k < T-1) {
		ret[k++] = cod[*pos_in_string];
		lex_advance(cod, pos_in_string);
	}

	if (k == T-1)
		error(ERROR_EXCEEDE_T, "lex string", T-1, 0, "");

	ret[k] = 0;
}

void
lex_get_string_in_quotes(char *cod, int *pos_in_string, int linia, char *filename, char *ret)
{
	/* ia un string dintre "
	 * functia trebuie sa fie apelata cand
	 * pos_in_string este fix deasupra unui "
	 */
	char prev_char = 0;
	int k = 0;

	lex_advance(cod, pos_in_string);
	while (cod[*pos_in_string]) {
		if (cod[*pos_in_string] == '"' && prev_char != '\\')
			break;

		if (prev_char == '\\') {
			SWITCH_ESCAPE_CHARACTER(cod[*pos_in_string], ret, k++);
			prev_char = ret[k];
		} else {
			if (cod[*pos_in_string] != '\\')
				ret[k++] = cod[*pos_in_string];
			prev_char = cod[*pos_in_string];
		}

		lex_advance(cod, pos_in_string);
	}

	if (cod[*pos_in_string] != '"')
		error(ERROR_STRING_NOT_FINISHED, linia, filename);

	ret[k] = 0;
}
