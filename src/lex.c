#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lex.h>
#include <errors.h>
#include <global_vars_declared.h>

#define SWITCH_ESCAPE_CHARACTER(CHR, STR, K)         \
	do {                                         \
		switch ((CHR)) {                     \
		case 'a': (STR)[(K)] = '\a'; break;  \
		case 'b': (STR)[(K)] = '\b'; break;  \
		case 'f': (STR)[(K)] = '\f'; break;  \
		case 'n': (STR)[(K)] = '\n'; break;  \
		case 't': (STR)[(K)] = '\t'; break;  \
		case 'r': (STR)[(K)] = '\r'; break;  \
		case 'v': (STR)[(K)] = '\v'; break;  \
		case '\\': (STR)[(K)] = '\\'; break; \
		case '\'': (STR)[(K)] = '\''; break; \
		case '\"': (STR)[(K)] = '\"'; break; \
		case '?': /* fallthrough */          \
		default: (STR)[(K)] = '?'; break;    \
		}                                    \
	} while (0)

void
lex(char *filename, int ismain)
{
	char *cod = NULL;
	int pos_in_string = 0, indent = 0, linia = 1;

	/* cod este alocat dinamic */
	lex_verify_file_loop(filename);
	cod = lex_get_cod(filename, ismain);

	while (cod[pos_in_string]) {
		if (strchr(" \t", cod[pos_in_string])) {
			lex_advance(cod, &pos_in_string);
		} else if (cod[pos_in_string] == '\\') {
			linia++;
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '#') {
			while (cod[pos_in_string] && cod[pos_in_string] != '\n' &&
			       cod[pos_in_string] != EOF)
				lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '+') {
			insert_token(TT_PLUS, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '-') {
			insert_token(TT_MINUS, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '*') {
			insert_token(TT_MULTIPLY, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '/') {
			insert_token(TT_DIVIDE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '%') {
			insert_token(TT_MOD, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '^') {
			insert_token(TT_EXPONENTIAL, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		/* este necesar ca <-, <=, >=, != sa fie deasupra <, >, si ! */
 		} else if (TWOCHAR('<', '-')) {
			insert_token(TT_ASSIGN, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (TWOCHAR('<', '=')) {
			insert_token(TT_LE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (TWOCHAR('>', '=')) {
			insert_token(TT_GE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (TWOCHAR('!', '=')) {
			insert_token(TT_NE, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '<') {
			insert_token(TT_L, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '>') {
			insert_token(TT_G, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '=') {
			insert_token(TT_E, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '!') {
			insert_token(TT_NOT, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '(') {
			insert_token(TT_LPAR, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == ')') {
			insert_token(TT_RPAR, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '[') {
			insert_token(TT_LBRACKET, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == ']') {
			insert_token(TT_RBRACKET, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == ',') {
			insert_token(TT_SEPEXP, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '|') {
			indent++;
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == EOF) {
			insert_token(TT_EOF, "", linia, indent, filename);
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '\n' || cod[pos_in_string] == ';') { 
			if (tokens_end != -1 &&
			    tokens[tokens_end].type !=  TT_NEXT)
				insert_token(TT_NEXT, "", linia, indent, filename);
			if (cod[pos_in_string] == '\n') {
				linia++;
				indent = 0;
			}
			lex_advance(cod, &pos_in_string);

		} else if (cod[pos_in_string] == '\'') {
			char next_char = 0, next_next_char = 0, temp[2] = "";

			lex_advance(cod, &pos_in_string);
			next_char = cod[pos_in_string];
			lex_advance(cod, &pos_in_string);
			next_next_char = cod[pos_in_string];

			if (next_char == '\\')
				lex_advance(cod, &pos_in_string);

			if ((next_char != '\\' && next_next_char != '\'') ||
			    (next_char == '\\' && cod[pos_in_string] != '\''))
				error(ERROR_CHAR_LEX_UNDEFINED_WELL, linia, filename);

			if (next_char == '\\')
				SWITCH_ESCAPE_CHARACTER(next_next_char, temp, 0);
			else
				temp[0] = next_char;

			temp[1] = 0;
			lex_advance(cod, &pos_in_string);
			insert_token(TT_CHAR, temp, linia, indent, filename);

		} else if (cod[pos_in_string] == '"') {
			char temp[T];

			lex_get_string_in_quotes(cod, &pos_in_string, linia,
			                         filename, temp);
			lex_advance(cod, &pos_in_string);
			insert_token(TT_STRING, temp, linia, indent,
			             filename);

		} else if (strchr(DIGITS, cod[pos_in_string])) {
			char temp[T];

			lex_get_number_string(cod, &pos_in_string, linia,
			                      filename, temp);
			insert_token(TT_NUMAR, temp, linia, indent, filename);

		} else if (strchr(CHARS, cod[pos_in_string])) {
			char temp[T];

			lex_get_string(cod, &pos_in_string, temp);
			if (strcmp(temp, "daca") == 0) {
				insert_token(TT_DACA, "", linia, indent, filename);
			} else if (strcmp(temp, "altfel") == 0) {
				insert_token(TT_ALTFEL, "", linia, indent, filename);
			} else if (strcmp(temp, "cat") == 0) {
				if (cod[pos_in_string]) {
					lex_advance(cod, &pos_in_string);
					lex_get_string(cod, &pos_in_string, temp);
					if (strcmp(temp, "timp") == 0)
						insert_token(TT_CATTIMP, "",
						             linia, indent, filename);
					else
						error(ERROR_CATTIMP_SYNTAX,
						      linia, filename);
				} else {
					error(ERROR_CATTIMP_SYNTAX, linia,
					      filename);
				}
			} else if (strcmp(temp, "pana") == 0) {
				if (cod[pos_in_string]) {
					lex_advance(cod, &pos_in_string);
					lex_get_string(cod, &pos_in_string, temp);
					if (strcmp(temp, "cand") == 0)
						insert_token(TT_PANACAND, "",
						             linia, indent,
							     filename);
					else
						error(ERROR_PANACAND_SYNTAX,
						      linia, filename);
				} else {
					error(ERROR_PANACAND_SYNTAX, linia,
					      filename);
				}
			} else if (strcmp(temp, "pentru") == 0) {
				insert_token(TT_PENTRU, "", linia, indent, filename);
			} else if (strcmp(temp, "repeta") == 0 ) {
				insert_token(TT_REPETA, "", linia, indent, filename);
			} else if (strcmp(temp, "subprogramul") == 0) {
				insert_token(TT_SUBPROGRAM, "", linia, indent,
				             filename);
			} else if (strcmp(temp, "returneaza") == 0 ||
			           strcmp(temp, "intoarce") == 0) {
				insert_token(TT_RETURN, "", linia, indent, filename);
			} else if (strcmp(temp, "atunci") == 0 ||
			           strcmp(temp, "executa") == 0 ||
			           strcmp(temp, "face") == 0) {
				insert_token(TT_BEGIN, "", linia, indent, filename);
			} else if (strcmp(temp, "si") == 0) {
				insert_token(TT_SI, "", linia, indent, filename);
			} else if (strcmp(temp, "sau") == 0) {
				insert_token(TT_SAU, "", linia, indent, filename);
			} else if (strcmp(temp, "not") == 0) {
				insert_token(TT_NOT, "", linia, indent, filename);
			} else if (strcmp(temp, "citeste") == 0) {
				insert_token(TT_CITESTE, "", linia, indent, filename);
			} else if (strcmp(temp, "citestetot") == 0) {
				insert_token(TT_CITESTETOT, "", linia, indent,
				             filename);
			} else if (strcmp(temp, "scrie") == 0) {
				insert_token(TT_SCRIE, "", linia, indent, filename);
			} else if (strcmp(temp, "TV_NUMAR") == 0) {
				insert_token(TT_TIP, "1", linia, indent, filename);
			} else if (strcmp(temp, "TV_CHAR") == 0) {
				insert_token(TT_TIP, "2", linia, indent, filename);
			} else if (strcmp(temp, "TV_VECTOR") == 0) {
				insert_token(TT_TIP, "3", linia, indent, filename);
			} else if (strcmp(temp, "TV_TIP") == 0) {
				insert_token(TT_TIP, "4", linia, indent, filename);
			} else if (strcmp(temp, "TV_ERROR") == 0) {
				insert_token(TT_TIP, "0", linia, indent, filename);
			} else if (strcmp(temp, "sursa") == 0) {
				char tmpfilename[T];

				/* compactare pana la " sau < */
				while (cod[pos_in_string] && cod[pos_in_string] != '"')
					lex_advance(cod, &pos_in_string);

				if (cod[pos_in_string] == '"')
					lex_get_string_in_quotes(cod, &pos_in_string,
					                         linia, filename,
								 tmpfilename);
				else
					error("sursa definita gresit la linia %d",
					      linia);
				lex_advance(cod, &pos_in_string);
				lex(tmpfilename, 0);
			} else {
				insert_token(TT_VAR, temp, linia, indent, filename);
			}
		} else {
			error(ERROR_UNKNOWN_CHAR_FORMAT, cod[pos_in_string],
			      linia, filename);
		}
	}

	free(cod);
	lex_pop_from_file_stack();
}

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

struct files *
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

char *
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

