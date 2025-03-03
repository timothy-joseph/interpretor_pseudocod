/* TODO: make break up parse_subprogram into smaller functions */
#include "parse.h"

/* functii ajuatatoare pentru parse */
struct node*
parse_daca(int start, int indent, int tt_next_pos)
{
	/* expr0: conditine, expr1: instructiuni pt adevarat,
	 * expr2opt: instructiuni pt false
	 */
	struct node *ret = NULL;
	int k, tt_begin_pos = start;
	has_else = 0;

	for (k = start; k <= tt_next_pos; k++) {
		if (tokens[k].type == TT_BEGIN) {
			tt_begin_pos = k;
			break;
		}
	}

	if (k == tt_next_pos + 1)
		error(ERROR_BEGIN_FORMAT, tokens[start].linia,
		      tokens[start].filename);
	if (start + 1 > tt_begin_pos - 1)
		error(ERROR_FARA_EXPRESIE_FORMAT, tokens[start].linia,
		      tokens[start].filename);

	ret = new_node(TN_DACA, "", tokens[start].linia, tokens[start].filename);
	ret->expresii[0] = parse_expression(start + 1, tt_begin_pos - 1);

	if (tokens[tt_begin_pos+1].type == TT_NEXT)
		ret->expresii[1] = parse(tt_next_pos + 1, indent + 1);
	else
		error(ERROR_INSTRUCTION_SAME_LINE, tokens[start].linia,
		      tokens[start].filename);

	if (has_else) {
		has_else = 0;

		/* else face exceptia la regula ca urmatoarea
		 * instructiune trebuie sa fie pe o linie diferita
		 */
		if (tokens[indent_change_pos+1].type == TT_NEXT)
			ret->expresii[2] = parse(indent_change_pos + 2, indent + 1);
		else
			ret->expresii[2] = parse(indent_change_pos + 1, indent + 1);
	}

	ret->next = parse(indent_change_pos, indent);
	return ret;
}

struct node*
parse_while(int start, int indent, int tt_next_pos)
{
	/* expr0: conditie, expr1: instructiuni pt adevarat */
	struct node *ret = NULL;
	int k, tt_begin_pos = start;

	for (k = start; k <= tt_next_pos; k++) {
		if (tokens[k].type == TT_BEGIN) {
			tt_begin_pos = k;
			break;
		}
	}

	if (k == tt_next_pos + 1) {
		indent_change_pos = start;
		/* pentru repeta pana cand */
		if (tokens[start].type == TT_CATTIMP)
			type_repeta = 1;
		else
			type_repeta = 2;
		return NULL;
	}

	if (start + 1 > tt_begin_pos - 1)
		error(ERROR_FARA_EXPRESIE_FORMAT, tokens[start].linia,
		      tokens[start].filename);

	if (tokens[start].type == TT_CATTIMP)
		ret = new_node(TN_CATTIMP, "", tokens[start].linia,
		               tokens[start].filename);
	else
		ret = new_node(TN_PANACAND, "", tokens[start].linia,
		               tokens[start].filename);

	ret->expresii[0] = parse_expression(start + 1, tt_begin_pos - 1);
	if (tokens[tt_begin_pos+1].type == TT_NEXT)
		ret->expresii[1] = parse(tt_next_pos + 1, indent + 1);
	else
		error(ERROR_INSTRUCTION_SAME_LINE, tokens[start].linia,
		      tokens[start].filename);

	ret->next = parse(indent_change_pos, indent);
	return ret;
}

struct node*
parse_repeta(int start, int indent, int tt_next_pos)
{
	/* expr0: conditia, expr1: instructiuni pt adevarat */
	struct node *ret = NULL;
	int k, tt_next_after_indent_change_pos = start;

	type_repeta = 0;
	ret = new_node(0, "", tokens[start].linia, tokens[start].filename);

	if (tokens[start+1].type == TT_NEXT)
		ret->expresii[1] = parse(tt_next_pos + 1, indent + 1);
	else
		error(ERROR_INSTRUCTION_SAME_LINE, tokens[start].linia,
		      tokens[start].filename);

	switch (type_repeta) {
	case 0:
		error(ERROR_REPETA_FORMAT, tokens[start].linia,
	              tokens[start].filename);
		break;
	case 1:
		ret->type = TN_REPETACATTIMP;
		break;
	case 2:
		ret->type = TN_REPETAPANACAND;
		break;
	}

	for (k = indent_change_pos; k <= tokens_end; k++) {
		tokens[start] = tokens[k];
		if (tokens[start].type == TT_NEXT || tokens[start].type == TT_EOF) {
			tt_next_after_indent_change_pos = k;
			break;
		}
	}

	ret->expresii[0] = parse_expression(indent_change_pos + 1,
	                                    tt_next_after_indent_change_pos - 1);

	ret->next = parse(tt_next_after_indent_change_pos + 1, indent);
	return ret;
}

struct node*
parse_pentru(int start, int indent, int tt_next_pos)
{
	/* expr0: expresia de inceput,
	 * expr1: conditia de continuare,
	 * expr2: ceea ce se intampla la final de loop
	 * expr3: instructiuni pentru adevarat
	 */
	struct node *ret = NULL;
	int k, tt_sepexp_count = 0, tt_sepexp_pos[2] = {0}, tt_begin_pos;

	for (k = start; k <= tt_next_pos; k++) {
		if (tokens[k].type == TT_SEPEXP) {
			tt_sepexp_count++;
			if (tt_sepexp_count > 2)
				error(ERROR_PENTRU_SEPEXP_FORMAT,
				      tokens[start].linia,
				      tokens[start].filename);

			tt_sepexp_pos[tt_sepexp_count-1] = k;
		} else if (tokens[k].type == TT_BEGIN) {
			tt_begin_pos = k;
			break;
		}
	}

	if (tt_sepexp_count < 2)
		error(ERROR_PENTRU_SEPEXP_FORMAT, tokens[start].linia,
		      tokens[start].filename);

	ret = new_node(TN_PENTRU, "", tokens[start].linia, tokens[start].filename);

	ret->expresii[0] = parse_expression(start + 1, tt_sepexp_pos[0] - 1);
	ret->expresii[1] = parse_expression(tt_sepexp_pos[0] + 1,
	                                    tt_sepexp_pos[1] - 1);
	ret->expresii[2] = parse_expression(tt_sepexp_pos[1] + 1, tt_begin_pos - 1);

	if (tokens[tt_begin_pos+1].type == TT_NEXT)
		ret->expresii[3] = parse(tt_next_pos + 1, indent + 1);
	else
		error(ERROR_INSTRUCTION_SAME_LINE, tokens[start].linia,
		      tokens[start].filename);

	ret->next = parse(indent_change_pos, indent);
	return ret;
}

struct node*
parse_subprogram(int start, int indent, int tt_next_pos)
{
	/* nume subpgrogram, variabile, definitie */
	int k, tt_lpar_pos = 0, tt_rpar_pos = 0, tt_begin_pos = 0,
	    tt_sepexp_prev_pos = 0, var_names_end = 0;
	char var_names[T][T] = {0};
	struct variable *tmp = NULL;
	struct node *cod = NULL;

	if (tokens[start+1].type != TT_VAR)
		error(ERROR_NUME_SUBPROGRAM, tokens[start].linia,
		      tokens[start].filename);
	if (tokens[start].indent > 0)
		error(ERROR_INDENT_SUBPROGRAM_FORMAT, tokens[start].linia,
		      tokens[start].filename);

	/* gasire pozitie a parantezelor si a cuvantului cheie
	 * de incepere a definirii subprogramului
	 * si verificare daca utilizatorul a folosit
	 * mai mult de un set de paranteze (eroare)
	 */
	for (k = start; k <= tt_next_pos; k++) {
		if (tokens[k].type == TT_LPAR) {
			if (tt_lpar_pos)
				error(ERROR_LPAR_SUBPROGRAM_FORMAT,
				      tokens[start].linia,
				      tokens[start].filename);
			tt_lpar_pos = k;
		} else if (tokens[k].type == TT_RPAR) {
			if (tt_rpar_pos)
				error(ERROR_RPAR_SUBPROGRAM_FORMAT,
				      tokens[start].linia,
				      tokens[start].filename);
			tt_rpar_pos = k;
		} else if (tokens[k].type == TT_BEGIN) {
			tt_begin_pos = k;
			break;
		}
	}

	if (k == tt_next_pos + 1)
		error(ERROR_BEGIN_FORMAT, tokens[start].linia,
		      tokens[start].filename);
	if (!tt_lpar_pos || !tt_rpar_pos)
		error(ERROR_PARANTHESIS_NOT_MATCHING_FORMAT,
		      tokens[start].linia, tokens[start].filename);

	/* variabile */
	if (tt_lpar_pos+1 == tt_rpar_pos) 
		goto subprogram_vars_skip;

	tt_sepexp_prev_pos = tt_lpar_pos;
	for (k = tt_lpar_pos; k <= tt_rpar_pos && var_names_end < T; k++) {
		if (tokens[k].type == TT_SEPEXP || k == tt_rpar_pos) {
			if (k - tt_sepexp_prev_pos != 2 ||
			    tokens[tt_sepexp_prev_pos+1].type != TT_VAR)
				error(ERROR_VARIABILE_SUBPROGRAM_FORMAT,
				      tokens[start].linia, tokens[start].filename);

			strncpy(var_names[var_names_end],
			        tokens[tt_sepexp_prev_pos+1].value, T);
			var_names_end++;
			tt_sepexp_prev_pos = k;
		}
	}

subprogram_vars_skip:
	/* cod */
	if (tokens[tt_begin_pos+1].type == TT_NEXT)
		cod = parse(tt_next_pos + 1, indent + 1);
	else
		error(ERROR_INSTRUCTION_SAME_LINE, tokens[start].linia,
		      tokens[start].filename);

	/* facere subrogram, inserare in variabile si inserare in
	 * garbage collection
	 */
	tmp = insert_or_get_by_key((void **)global_var_map, tokens[start+1].value,
	                           strlen(tokens[start+1].value),
				   variable_map_utilities);
	tmp->type = TV_FUNCTIE;
	tmp->value.v = make_subprogram(tokens[start+1].value, (char **)var_names,
	                               var_names_end, cod);
	increase_garbage_recursive(tmp->value.v, tmp->type == TV_VECTOR, 0);

	/* sarire la urmatoarea parte de cod (subprogramul nu apare in
	 * parse_tree principal)
	 */
	return parse(indent_change_pos, indent);
}

struct node*
parse_citeste(int start, int indent, int tt_next_pos)
{
	struct node *ret = NULL;
	int k, tt_prev_sepexp_pos = start, i = 0;

	if (start + 1 > tt_next_pos - 1)
		error(ERROR_FARA_EXPRESIE_FORMAT, tokens[start].linia,
		      tokens[start].filename);

	if (tokens[start].type == TN_CITESTETOT)
		ret = new_node(TN_CITESTETOT, "", tokens[start].linia,
		               tokens[start].filename);
	else
		ret = new_node(TN_CITESTE, "", tokens[start].linia,
		               tokens[start].filename);

	for (k = start; k <= tt_next_pos && i < T; k++) {
		if (tokens[k].type == TT_SEPEXP || tokens[k].type == TT_NEXT) {
			ret->expresii[i] = parse_expression(tt_prev_sepexp_pos + 1,
			                                    k - 1);
			i++;
			tt_prev_sepexp_pos = k;
		} 

		if (tokens[start].type == TN_CITESTETOT && i > 1)
			error(ERROR_CITESTETOT_PARAMETRII, tokens[start].linia,
			      tokens[start].filename);
		if (tokens[k].type == TT_NEXT)
			break;
	}
	if (i == T)
		error(ERROR_EXCEEDE_T, "parse citeste", T, tokens[start].linia,
		      tokens[start].filename);

	ret->next = parse(tt_next_pos+1, indent);
	return ret;
}

struct node*
parse_scrie(int start, int indent, int tt_next_pos)
{
	struct node *ret = NULL;
	int k, tt_prev_sepexp_pos = start, i = 0, paranthesis = 0;

	if (start + 1 > tt_next_pos - 1)
		error(ERROR_FARA_EXPRESIE_FORMAT, tokens[start].linia,
		      tokens[start].filename);

	ret = new_node(TN_SCRIE, "", tokens[start].linia, tokens[start].filename);
	for (k = start; k <= tt_next_pos && i < T; k++) {
		if (tokens[k].type == TT_LPAR) {
			paranthesis++;
		} else if (tokens[k].type == TT_RPAR) {
			paranthesis--;
			if (paranthesis < 0)
				error(ERROR_PARANTHESIS_NOT_MATCHING_FORMAT,
				      tokens[start].linia,
				      tokens[start].filename);
		} else if ((tokens[k].type == TT_SEPEXP ||
		           tokens[k].type == TT_NEXT) &&
		           !paranthesis) {
			ret->expresii[i] = parse_expression(tt_prev_sepexp_pos + 1,
			                                    k - 1);
			i++;
			tt_prev_sepexp_pos = k;
		}
	}
	if (i == T)
		error(ERROR_EXCEEDE_T, "parse scrie", T, tokens[start].linia,
		      tokens[start].filename);
	if (paranthesis)
		error(ERROR_PARANTHESIS_NOT_MATCHING_FORMAT,
		      tokens[start].linia, tokens[start].filename);

	ret->next = parse(tt_next_pos + 1, indent);
	return ret;
}

/* functii ajutatoare pentru parse_expression */
struct node*
parse_data(int pos)
{
	struct node *ret = NULL;

	if (tokens[pos].type == TT_NUMAR || tokens[pos].type == TT_VAR ||
	    tokens[pos].type == TT_CHAR || tokens[pos].type == TT_TIP) {
		ret = new_node(tokens[pos].type, tokens[pos].value,
		               tokens[pos].linia, tokens[pos].filename);
		return ret;
	} else if (tokens[pos].type == TT_STRING) {
		int i;

		ret = new_node(TN_VECTOR, "", tokens[pos].linia,
		               tokens[pos].filename);
		for (i = 0; tokens[pos].value[i] && i < T-1; i++)
			/* TODO: &tokens[pos].value[i] nu este sigur din punct de vedere al memoriei,
			 * TODO: alocare de memorie
			 */
			ret->expresii[i] = new_node(TN_CHAR, &tokens[pos].value[i],
			                            tokens[pos].linia, tokens[pos].filename);
		if (i == T-1)
			error(ERROR_EXCEEDE_T, "parse string", T-1,
			      tokens[pos].linia, tokens[pos].filename);

		ret->expresii[i] = new_node(TN_CHAR, &tokens[pos].value[i],
		                            tokens[pos].linia, tokens[pos].filename);
		return ret;
	} else {
		error(ERROR_EXPERSSION_MALFORMED_FORMAT, tokens[pos].linia,
		      tokens[pos].filename);
		return NULL;
	}
}

struct node*
parse_vector(int start, int end)
{
	/* aceasta functie este apelata din interiorul
	 * parse_expression si s-a verificat deja daca
	 * are sau numarul potrivit de paranteze
	 * cu is_balanced_expression
	 */
	struct node *ret = NULL;
	int k, i = 0, tt_lpar_pos = start, tt_rpar_pos = end,
	    tt_sepexp_prev_pos = start, paranthesis = 0;

	ret = new_node(TN_VECTOR, "", tokens[start].linia, tokens[start].filename);
	for (k = tt_lpar_pos; k <= tt_rpar_pos && i < T; k++) {
		if (k != tt_lpar_pos && tokens[k].type == TT_LPAR) {
			paranthesis++;
		} else if (k != tt_rpar_pos && tokens[k].type == TT_RPAR) {
			paranthesis--;
		} else if (!paranthesis &&
		           (tokens[k].type == TT_SEPEXP ||
			   tokens[k].type == TT_RPAR)) {
			ret->expresii[i] = parse_expression(tt_sepexp_prev_pos + 1, k - 1);
			i++;
			tt_sepexp_prev_pos = k;
		}
		ret->expresii[i] = NULL;
	}
	if (i == T)
		error(ERROR_EXCEEDE_T, "parse vector", T, tokens[start].linia,
		      tokens[start].filename);

	return ret;
}

struct node*
parse_call(int start, int end)
{
	/* parantezele au fost deja verificate de is_balanced_expression
	 * aceasta functie este apelata doar in parse_expression si est
	 * apelata dupa is_balanced_expression
	 */
	struct node *ret = NULL;
	int k, i = 0, tt_lpar_pos = start+1, tt_rpar_pos = 0,
	    tt_sepexp_prev_pos = start, paranthesis = 0;

	for (k = end; k >= tt_lpar_pos; k--) {
		if (tokens[k].type == TT_RPAR) {
			tt_rpar_pos = k;
			break;
		}
	}

	ret = new_node(TN_CALL, tokens[start].value, tokens[start].linia,
	               tokens[start].filename);
	tt_sepexp_prev_pos = tt_lpar_pos;
	for (k = tt_lpar_pos; k <= tt_rpar_pos && i < T; k++) {
		if (k != tt_lpar_pos && tokens[k].type == TT_LPAR) {
			paranthesis++;
		} else if (k != tt_rpar_pos && tokens[k].type == TT_RPAR) {
			paranthesis--;
		} if (!paranthesis && (tokens[k].type == TT_SEPEXP || k == tt_rpar_pos)) {
			ret->expresii[i] = parse_expression(tt_sepexp_prev_pos+1, k-1);
			i++;
			tt_sepexp_prev_pos = k;
		}
		ret->expresii[i] = NULL;
	}
	if (i == T)
		error(ERROR_EXCEEDE_T, "parse call", T, tokens[start].linia,
		      tokens[start].filename);
	return ret;
}

struct node*
parse_indexing(int start, int end)
{
	/* parantezele au fost deja verificate de is_balanced_expression
	 * aceasta functie este apelata doar in parse_expression si est
	 * apelata dupa is_balanced_expression
	 */
	struct node *ret = NULL;
	int k, i = 0, tt_lbracket_pos = start + 1, tt_rbracket_pos = 0,
	    prev_rbracket_pos = start, brackets = 0;

	ret = new_node(TN_INDEXING, tokens[start].value, tokens[start].linia,
	               tokens[start].filename);
	while (prev_rbracket_pos + 1 <= end &&
	       tokens[prev_rbracket_pos + 1].type == TT_LBRACKET && i < T - 1) {

		tt_lbracket_pos = prev_rbracket_pos + 1;

		for (k = tt_lbracket_pos; k <= end; k++) {
			if (tokens[k].type == TT_LBRACKET)
				brackets++;
			else if (tokens[k].type == TT_RBRACKET)
				brackets--;

			if (brackets < 0)
				error(ERROR_EXPERSSION_MALFORMED_FORMAT,
				      tokens[k].linia, tokens[k].filename);

			if (brackets == 0 && tokens[k].type == TT_RBRACKET) {
				tt_rbracket_pos = k;
				break;
			}
		}

		ret->expresii[i] = parse_expression(tt_lbracket_pos + 1,
		                                    tt_rbracket_pos - 1);
		if (ret->expresii[i] == NULL)
			error(ERROR_INDEXING_PARSE_NO_INDEX, tokens[k].linia,
			      tokens[k].filename);
		i++;
		prev_rbracket_pos = tt_rbracket_pos;
	}
	if (i == T-1)
		error(ERROR_EXCEEDE_T, "parse indexing", T-1,
		      tokens[prev_rbracket_pos].linia,
		      tokens[prev_rbracket_pos].filename);

	ret->expresii[i] = NULL;
	return ret;
}

/* alte tipuri de functii ajuatatoare pentru etapa de parse */
struct node*
new_node(int type, char *value, int linia, char *filename)
{
	/* alocheaza memorie pentru un nod nou si initializeaza toate
	 * campurile cu 0
	 */
	int k;
	struct node *tmp = (struct node*)malloc(sizeof(struct node));

	if (tmp == NULL)
		error(ERROR_ALLOC_NODE);

	tmp->type = type;
	strncpy(tmp->value, value, T);
	tmp->linia = linia;
	strncpy(tmp->filename, filename, T);
	memset(tmp->expresii, 0, T * sizeof(struct node*));
	tmp->next = NULL;
	return tmp;
}

int
is_balanced_expression(int start, int end)
{
	/* verifica daca expresia este balansata intre start si end (adica
	 * daca parantezele sunt balansate)
	 */
	int k, paranthesis = 0;

	if (start == end)
		return 1;
	else if (start > end)
		return 0;
	for (k = start; k <= end; k++) {
		if (tokens[k].type == TT_LPAR) {
			paranthesis++;
		} else if (tokens[k].type == TT_RPAR) {
			paranthesis--;
			if (paranthesis < 0)
				return 0;
		}
	}
	return paranthesis == 0;
}

int
is_all_in_paranthesis_expression(int start, int end)
{
	/* verifica daca toate expresia dintre start si end este in paranteza */
	return is_balanced_expression(start+1, end-1) &&
	       tokens[start].type == TT_LPAR &&
	       tokens[end].type == TT_RPAR;
}

int
is_vector(int start, int end)
{
	/* verifica daca expresia dintre start si end este
	 * un vector adica (1, 2, 3)
	 */
	int i, ok = 0, paranthesis = 0;

	for (i = start; i <= end; i++) {
		if (tokens[i].type == TT_LPAR) {
			paranthesis++;
		} else if (tokens[i].type == TT_RPAR) {
			paranthesis--;
			if (paranthesis < 0)
				return 0;
		} else if (tokens[i].type == TT_SEPEXP) {
			ok = 1;
		}
	}
	return ok && tokens[start].type == TT_LPAR &&
	       tokens[end].type == TT_RPAR && !paranthesis;
}

int
get_pres_operator(struct token tmp)
{
	/* da gradul simbolului (+, -, *, / etc)
	 * 0: assign
	 * 1: or
	 * 2: and
	 * 3: egal, inegal
	 * 4: <, >, <=, >=
	 * 5: +, -
	 * 7: *, /, %
	 * 8: ^
	 * 9: not
	 */

	switch (tmp.type) {
	case TT_ASSIGN:
		return 0;
	case TT_SAU:
		return 1;
	case TT_SI:
		return 2;
	case TT_E:
		return 3;
	case TT_NE:
		return 3;
	case TT_L:
		return 4;
	case TT_G:
		return 4;
	case TT_LE:
		return 4;
	case TT_GE:
		return 4;
	case TT_PLUS:
		return 5;
	case TT_MINUS:
		return 5;
	case TT_MULTIPLY:
		return 7;
	case TT_DIVIDE:
		return 7;
	case TT_MOD:
		return 7;
	case TT_EXPONENTIAL:
		return 8;
	case TT_NOT:
		return 9;
	default:
		return ERROR_PRESIDENCY_NUMBER;
	}
	return ERROR_PRESIDENCY_NUMBER;
}

void
free_parse_tree(struct node *root)
{
	/* elibereaza memoria alocata pentru parse tree */
	int k;

	if (root == NULL)
		return;
	for (k = 0; root->expresii[k] && k < T; k++)
		free_parse_tree(root->expresii[k]);
	free(root);
}
