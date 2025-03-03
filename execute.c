/* TODO: break up execute_citestetot, execute_citeste, execute_scrie, execute_call in smaller functions */
struct variable
execute_daca(struct node *root)
{
	/* cu verificare de returnare din subprogram */
	struct variable x = execute(root->expresii[0]);

	if (x.value.d || x.value.v)
		return execute(root->expresii[1]);
	else
		return execute(root->expresii[2]);
}

struct variable
execute_while(struct node *root, int p)
{
	struct variable ret = {0}, x = {0};

	/* pentru TN_CATTIMP si TN_PANACAND
	 * se poate folosi for, dar cred ca este mai
	 * ok asa deoarece se foloseste while in
	 * implementarea de while
	 *
	 * cu verificare daca s-a returnat din subprogram
	 */
	if (root->type == TN_CATTIMP) {
		x = execute(root->expresii[0]);
		while (p == stack_head->p && (x.value.d || x.value.v)) {
			ret = execute(root->expresii[1]);
			if (p == stack_head->p)
				x = execute(root->expresii[0]);
		}
	} else if (root->type == TN_PANACAND) {
		x = execute(root->expresii[0]);
		while (p == stack_head->p && (!x.value.d && !x.value.v)) {
			ret = execute(root->expresii[1]);
			if (p == stack_head->p)
				x = execute(root->expresii[0]);
		}
	} else if (root->type == TN_REPETACATTIMP) {
		do {
			ret = execute(root->expresii[1]);
			if (p == stack_head->p)
				x = execute(root->expresii[0]);
		} while (p == stack_head->p && (x.value.d || x.value.v));
	} else if (root->type == TN_REPETAPANACAND) {
		do {
			ret = execute(root->expresii[1]);
			if (p == stack_head->p)
				x = execute(root->expresii[0]);
		} while (p == stack_head->p && (!x.value.d && !x.value.v));
	}

	return ret;
}

struct variable
execute_pentru(struct node *root, int p)
{
	struct variable ret = {0}, y = {0};

	execute(root->expresii[0]);
	y = execute(root->expresii[1]);

	while (p == stack_head->p && (y.value.d || y.value.v)) {
		ret = execute(root->expresii[3]);

		if (p == stack_head->p) {
			execute(root->expresii[2]);
			y = execute(root->expresii[1]);
		}
	}

	return ret;
}

struct variable
execute_citestetot(struct node *root, struct variable **var_map)
{
	char in[T];
	int k;
	struct variable ret = {0}, y = {0}, *x = NULL;

	if (!(root->expresii[0]->type == TN_VAR ||
	    root->expresii[0]->type == TN_INDEXING))
		error(ERROR_ASSIGN_FORMAT, root->linia, root->filename);

	fgets(in, T, stdin);

	ret.type = TV_VECTOR;
	ret.dimensiune = strlen(in)+1;
	ret.value.v = (struct variable *)calloc(strlen(in) + 1,
	                                        sizeof(struct variable));
	if (ret.value.v == NULL)
		error(ERROR_VECTOR_NO_SPACE, root->linia, root->filename);

	increase_garbage_recursive((struct variable *)ret.value.v, ret.type == TV_VECTOR,
	                           ret.dimensiune);

	/* punerea valorilor in ret */
	for (k = 0; in[k]; k++) {
		y.type = TV_CHAR;
		y.value.d = in[k];
		varcpy(&(((struct variable *)(ret.value.v))[k]), &y);
	}
	y.type = TV_CHAR;
	y.value.d = 0;
	varcpy(&(((struct variable *)(ret.value.v))[k]), &y);

	/* gasirea locatiei (variabila sau tn_indexing) */
	if (root->expresii[0]->type == TN_VAR)
		x = tn_var_get_or_make(root->expresii[0], var_map);
	else
		x = tn_indexing_helper(root->expresii[0], var_map);

	GARBAGE_COUNT_DECREASE_WRAPPER(*x);
	varcpy(x, &ret);
	
	return ret;
}

struct variable
execute_citeste(struct node *root, struct variable **var_map)
{
	char in[T], format[13] = "", enumar = 1, dots = 0;
	int i = 0, j, k;
	struct variable ret = {0}, y = {0}, *x = NULL;

	/* variabila format are lungimea 13, deoarece T, fiind dat
	 * ca si unsigned int, are lungime maxima de 10 cifre/caractere
	 * se mai pune caracterul % si caracterul s si caracterul null la
	 * final, %u inseamna %d, dar unsigned
	 */
	snprintf(format, sizeof(format), "%%%us", (unsigned int)T);

	for (i = 0; root->expresii[i] && i < T; i++) {
		enumar = 1;
		if (!(root->expresii[i]->type == TN_VAR ||
		    root->expresii[i]->type == TN_INDEXING))
			error(ERROR_ASSIGN_FORMAT, root->linia, root->filename);

		scanf(format, in);
		for (j = 0; in[j]; j++) {
			if (!strchr(DIGITS, in[j]) && in[j] != '.')
				enumar = 0;
			if (in[j] == '.') {
				dots++;
				if (dots > 1)
					enumar = 0;
			}
		}

		/* punerea valorilor in ret */
		if (enumar) {
			ret.type = TV_NUMAR;
			ret.value.d = atof(in);
		} else if (strlen(in) == 1) {
			ret.type = TV_CHAR;
			ret.value.d = (double)(*in);
		} else {
			/* string */
			ret.type = TV_VECTOR;
			ret.dimensiune = strlen(in)+1;
			ret.value.v = (struct variable *)calloc(strlen(in)+1,
			                              sizeof(struct variable));
			if (ret.value.v == NULL)
				error(ERROR_VECTOR_NO_SPACE, root->linia,
				      root->filename);

			increase_garbage_recursive((struct variable *)ret.value.v,
			                           ret.type == TV_VECTOR, ret.dimensiune);

			for (k = 0; in[k]; k++) {
				y.type = TV_CHAR;
				y.value.d = in[k];
				varcpy(&(((struct variable *)(ret.value.v))[k]), &y);
			}
			y.type = TV_CHAR;
			y.value.d = 0;
			varcpy(&(((struct variable *)(ret.value.v))[k]), &y);
		}

		/* punerea valorilor in variabila data */
		if (root->expresii[i]->type == TN_VAR)
			x = tn_var_get_or_make(root->expresii[i], var_map);
		else
			x = tn_indexing_helper(root->expresii[i], var_map);

		GARBAGE_COUNT_DECREASE_WRAPPER(*x);
		varcpy(x, &ret);
	}
	if (i == T)
		error(ERROR_EXCEEDE_T, "execute citeste", T, root->linia,
		      root->filename);

	return ret;
}

struct variable
execute_scrie(struct node *root, struct variable **var_map)
{

	int i = 0, k = 0, toate_char = 1, to_collect = 0;
	struct variable ret = {0}, x = {0};

	for (i = 0; root->expresii[i] && i < T; i++) {
		/* verificare daca se creeaza un vector doar pentru scrie */
		if (root->expresii[i]->type == TN_VECTOR)
			to_collect = 1;

		x = execute(root->expresii[i]);
		if (x.type == TV_VECTOR) {
			/* determinare daca este vector de caractere sau vector normal */
			for (k = 0; k <= x.dimensiune; k++) {
				if (((struct variable *)(x.value.v))[k].type == TN_CHAR &&
				    ((struct variable *)(x.value.v))[k].value.d == 0)
					break;
				if (((struct variable *)(x.value.v))[k].type != TN_CHAR) {
					toate_char = 0;
					break;
				}
			}

			if (toate_char) {
				for (k = 0; k <= x.dimensiune; k++) {
					if (((struct variable *)(x.value.v))[k].type == TN_CHAR &&
					    ((struct variable *)(x.value.v))[k].value.d == 0)
						break;
					printf("%c", (char)(((struct variable *)(x.value.v))[k].value.d));
				}
			} else {
				error(ERROR_SCRIE_VECTOR, root->linia, root->filename);
			}
		} else if (x.type == TV_CHAR) {
			printf("%c", (char)x.value.d);
		} else if (x.type == TV_TIP) {
			switch ((int)floor(x.value.d)) {
			case TV_NUMAR: printf("TV_NUMAR"); break;
			case TV_CHAR: printf("TV_CHAR"); break;
			case TV_VECTOR: printf("TV_VECTOR"); break;
			case TV_TIP: printf("TV_TIP"); break;
			default: printf("TV_ERROR"); break;
			}
		} else {
			if (floor(x.value.d) == x.value.d)
				printf("%d", (int)x.value.d);
			else
				printf("%f", x.value.d);
		}
	}

	if (i == T)
		error(ERROR_EXCEEDE_T, "execute scrie", T, root->linia,
		      root->filename);
	if (to_collect)
		garbage_collect();
	
	return ret;
}

struct variable
execute_call(struct node *root, struct variable **var_map, int p)
{
	struct variable ret = {0};
	int k;

	for (k = 0; root->expresii[k] && k < T; k++); /* nr paramentrii */

	if (strcmp(root->value, "vector") == 0) {
		/* functia vector declara un vector unidimensional */
		struct variable x = {0};

		if (k > 1)
			error(ERROR_PREA_MULTE_ARGUMENT_DECLARARE_VECTOR,
			      root->linia, root->filename);

		x = execute(root->expresii[0]);
		if (x.type != TV_NUMAR)
			error(ERROR_NOT_NUMAR_DECLARARE_VECTOR, root->linia,
			      root->filename);

		ret.type = TV_VECTOR;
		ret.dimensiune = (int)floor(x.value.d);
		ret.value.v = (struct variable *)calloc((int)floor(x.value.d),
		                              sizeof(struct variable));
		if (ret.value.v == NULL)
			error(ERROR_VECTOR_NO_SPACE, root->linia,
			      root->filename);

		insert_or_get_by_key((void **)possible_garbage_map, ret.value.v,
		                     sizeof(ret.value.v), garbage_map_utilities);
	} else if (strcmp(root->value, "referinta") == 0) {
		struct variable *x = NULL;

		if (k > 1)
			error(ERROR_PREA_MULTE_ARGUMENTE_REFERENCE, root->linia,
			      root->filename);
		if (root->expresii[0]->type != TN_VAR)
			error(ERROR_REFERENCE_NOT_VAR, root->linia, root->filename);

		x = tn_var_get(root->expresii[0], var_map);
		ret.type = TV_VECTOR;
		ret.dimensiune = 1;
		ret.value.v = (void *)x;
		ret.from_referinta = 1;
	} else if (strcmp(root->value, "dimensiune") == 0) {
		struct variable x = {0};

		for (k = 0; root->expresii[k] && k < T; k++); /* nr paramentrii */
		if (k > 1)
			error(ERROR_DIMENSIUNE, root->linia, root->filename);

		x = execute(root->expresii[0]);
		if (x.type != TV_VECTOR)
			error(ERROR_DIMENSIUNE_NOT_VECTOR, root->linia,
			      root->filename);

		ret.value.d = x.dimensiune;
		ret.type = TV_NUMAR;
	} else if (strcmp(root->value, "floor") == 0) {
		struct variable x = {0};

		for (k = 0; root->expresii[k] && k < T; k++);
		if (k > 1)
			error(ERROR_FLOOR, root->linia, root->filename);

		x = execute(root->expresii[0]);
		if (x.type != TV_NUMAR)
			error(ERROR_FLOOR_NOT_NUMAR, root->linia, root->filename);

		ret.value.d = floor(x.value.d);
		ret.type = TV_NUMAR;
	} else if (strcmp(root->value, "sqrt") == 0) {
		struct variable x = {0};

		for (k = 0; root->expresii[k] && k < T; k++); /* nr paramentrii */
		if (k > 1)
			error(ERROR_SQRT, root->linia, root->filename);

		x = execute(root->expresii[0]);
		if (x.type != TV_NUMAR)
			error(ERROR_SQRT_NOT_NUMAR, root->linia, root->filename);
		if (x.value.d < 0)
			error(ERROR_SQRT_BELOW_ZERO, root->linia, root->filename);

		ret.value.d = sqrt(x.value.d);
		ret.type = TV_NUMAR;
	} else if (strcmp(root->value, "tip") == 0) {
		struct variable x = {0};

		for (k = 0; root->expresii[k] && k < T; k++); /* nr paramentrii */
		if (k > 1)
			error(ERROR_TIP_FUNCTION, root->linia, root->filename);

		x = execute(root->expresii[0]);
		ret.type = TV_TIP;
		ret.value.d = x.type;
	} else if (strcmp(root->value, "cast") == 0 ||
	           strcmp(root->value, "schimbare_tip") == 0) {
		struct variable x = {0}, y = {0};

		for (k = 0; root->expresii[k] && k < T; k++); /* nr paramentrii */
		if (k != 2)
			error(ERROR_CAST_FUNCTION, root->linia, root->filename);

		x = execute(root->expresii[0]);
		y = execute(root->expresii[1]);
		if (y.type != TV_TIP)
			error(ERROR_CAST_TIP, root->linia, root->filename);
		if (y.value.d != TV_NUMAR && y.value.d != TV_CHAR && y.value.d != TV_TIP)
			error(ERROR_CAST_INVALID_TYPE, root->linia, root->filename);

		ret.type = y.value.d;
		ret.value = x.value;
	} else {
		/* subprogram definit de catre utilizator */
		struct subprogram *f = NULL;
		struct variable *var_map_pt_subprogram[MAP_CONSTANT] = {0};
		struct variable *x = find_by_key((void **)global_var_map, root->value,
		                          strlen(root->value), variable_map_utilities);

		if (x == NULL || x->type != TV_FUNCTIE)
			error(ERROR_NEDEFINIT_SUBPROGRAM_FORMAT, root->value,
			      root->linia, root->filename);
		f = (struct subprogram *)x->value.v;
		if (f == NULL)
			error(ERROR_NEDEFINIT_SUBPROGRAM_FORMAT, root->value,
			      root->linia, root->filename);

		/* verificare numar parametrii */
		if (k != f->var_names_end)
			error(ERROR_USER_DEFINED_FUNCTION_PARAMETERS, f->name,
			      root->linia, root->filename);

		/* adaugarea parametriilor */
		for (k = 0; root->expresii[k] && k < f->var_names_end; k++) {
			struct variable *tmp = NULL;
			struct variable y = execute(root->expresii[k]);

			tmp = insert_or_get_by_key((void **)var_map_pt_subprogram,
			                           f->var_names[k], strlen(f->var_names[k]),
						   variable_map_utilities);

			GARBAGE_COUNT_INCREASE_WRAPPER(y);
			varcpy(tmp, &y);
		}

		/* inserearea in stack si lansarea in executare */
		insert_stack(var_map_pt_subprogram);
		ret = execute(f->cod);

		/* subprogramul nu returneaza cu return si sa termina (root din
		 * apel a devenit null, deoarece a avansat prea mult)
		 */
		if (stack_head && stack_head->next && p != stack_head->p) {
			/* pop_stack apeleaza free_variable_list care elibereaza memoria
			 * si are si etape de garbage collection
			 */
			pop_stack();
			memset(&ret, 0, sizeof(ret));
		}
	}

	return ret;
}

struct variable
execute_operators(struct node *root, struct variable **var_map)
{
	struct variable ret = {0}, x = {0}, y = {0};

	switch (root->type) {
	case TN_ASSIGN:
		ret = execute_assign(root, var_map);
		break;
	case TN_SAU:
		BINARY_OPERATOR_LOGIC(||);
		break;
	case TN_SI:
		BINARY_OPERATOR_LOGIC(&&);
		break;
	case TN_E:
		BINARY_OPERATOR_NUMERIC(==);
		break;
	case TN_NE:
		BINARY_OPERATOR_NUMERIC(!=);
		break;
	case TN_L:
		BINARY_OPERATOR_NUMERIC(<);
		break;
	case TN_G:
		BINARY_OPERATOR_NUMERIC(>);
		break;
	case TN_LE:
		BINARY_OPERATOR_NUMERIC(<=);
		break;
	case TN_GE:
		BINARY_OPERATOR_NUMERIC(>=);
		break;
	case TN_PLUS:
		BINARY_OPERATOR_NUMERIC(+);
		break;
	case TN_MINUS:
		BINARY_OPERATOR_NUMERIC(-);
		break;
	case TN_MULTIPLY:
		BINARY_OPERATOR_NUMERIC(*);
		break;
	case TN_DIVIDE:
		BINARY_OPERATOR_NUMERIC(/);
		break;
	case TN_MOD:
		/* nu se poate folosi BINARY_OPERATOR_NUMERIC */
		x = execute(root->expresii[0]);
		y = execute(root->expresii[1]);
		if ((x.type == TV_NUMAR || x.type == TV_CHAR) &&
		    (y.type == TV_NUMAR || y.type == TV_CHAR))
			ret.value.d = (int)x.value.d % (int)y.value.d;
		else
			error(ERROR_NOT_NUMAR, root->type, root->linia,
			      root->filename);
		ret.type = TV_NUMAR;
		break;
	case TN_EXPONENTIAL:
		/* nu se poate folosi BINARY_OPERATOR_NUMERIC */
		x = execute(root->expresii[0]);
		y = execute(root->expresii[1]);
		if ((x.type == TV_NUMAR || x.type == TV_CHAR) &&
		    (y.type == TV_NUMAR || y.type == TV_CHAR))
			ret.value.d = pow(x.value.d, y.value.d);
		else
			error(ERROR_NOT_NUMAR, root->type, root->linia,
			      root->filename);
		ret.type = TV_NUMAR;
		break;
	case TN_NOT:
		x = execute(root->expresii[1]);
		if (x.type == TV_NUMAR || x.type == TV_CHAR)
			ret.value.d = !x.value.d;
		else 
			ret.value.d = !x.value.v;
		ret.type = TV_NUMAR;
		break;
	}

	return ret;
}
struct variable
execute_assign(struct node *root, struct variable **var_map)
{
	struct variable ret = {0}, y = {0}, *x = NULL;

	if (!(root->expresii[0]->type == TN_VAR ||
	    root->expresii[0]->type == TN_INDEXING))
		error(ERROR_ASSIGN_FORMAT, root->linia, root->filename);

	y = execute(root->expresii[1]);

	GARBAGE_COUNT_INCREASE_WRAPPER(y);

	if (root->expresii[0]->type == TN_VAR)
		x = tn_var_get_or_make(root->expresii[0], var_map);
	else
		x = tn_indexing_helper(root->expresii[0], var_map);

	GARBAGE_COUNT_DECREASE_WRAPPER(*x);
	varcpy(x, &y);
	varcpy(&ret, x);

	return ret;
}

struct variable
execute_vector(struct node *root)
{
	struct variable ret = {0};
	int k;

	for (k = 0; root->expresii[k]; k++); /* nr expresii */

	ret.type = TV_VECTOR;
	ret.dimensiune = k;
	ret.value.v = (struct variable *)calloc(k, sizeof(struct variable));

	if (ret.value.v == NULL)
		error(ERROR_VECTOR_NO_SPACE, root->linia, root->filename);

	insert_or_get_by_key((void **)possible_garbage_map, ret.value.v,
	                     sizeof(ret.value.v), garbage_map_utilities);

	for (k = 0; root->expresii[k] && k < T; k++) {
		struct variable y = execute(root->expresii[k]);
		varcpy(&((struct variable *)(ret.value.v))[k], &y);
	}

	return ret;
}

struct variable*
tn_var_get(struct node *root, struct variable **var_map)
{
	/* folosit in TN_VAR si in TN_CALL referinta pentru a gasi o variabila */
	struct variable *x = find_by_key((void **)var_map, root->value,
	                          strlen(root->value),
				  variable_map_utilities);

	if (x == NULL) {
		if (stack_head->p != 0)
			x = find_by_key((void **)global_var_map, root->value,
			                strlen(root->value),
					variable_map_utilities);
		if (x == NULL)
			error(ERROR_VARIABLE_UNDECLARED_FORMAT, root->value,
			      root->linia, root->filename);
	}
	return x;
}

struct variable*
tn_var_get_or_make(struct node *root, struct variable **var_map)
{
	/* folosit in TN_CITESTETOT, TN_CITESTE si TN_ASSIGN pentru a gasi o variabila in 
	 * in lista de variabile curente sau in lista de variabile globale
	 * daca nu este gasita in niciuna din ele se creeaza o variabila in lista
	 * de variabile curente
	 */
	struct variable *x = find_by_key((void **)var_map, root->value,
	                          strlen(root->value), variable_map_utilities);

	if (x == NULL) {
		x = find_by_key((void **)global_var_map, root->value,
		                strlen(root->value), variable_map_utilities);
		if (x == NULL) {
			x = insert_or_get_by_key((void **)var_map, root->value,
			                         strlen(root->value),
					         variable_map_utilities);
			if (x == NULL)
				error(ERROR_MEMORIE_VARIABILA_FORMAT, root->linia,
				      root->filename);
		}
	}
	return x;
}

struct variable*
tn_indexing_helper(struct node *root, struct variable **var_map)
{
	/* folosit pentru indexare in execute() */
	int k;
	struct variable y, *x = find_by_key((void **)var_map, root->value,
	                             strlen(root->value), variable_map_utilities);
	if (x == NULL)
		x = find_by_key((void **)global_var_map, root->value,
		                strlen(root->value), variable_map_utilities);

	for (k = 0; root->expresii[k] && k < T; k++) {
		y = execute(root->expresii[k]);
		if (x == NULL)
			error(ERROR_VARIABLE_UNDECLARED_FORMAT, root->value,
			      root->linia, root->filename);
		if (x->type != TV_VECTOR)
			error(ERROR_INDEXING_NON_VECTOR, root->value,
			      root->linia, root->filename);
		if (y.type != TV_NUMAR)
			error(ERROR_INDEXING_NOT_NUMAR, root->linia,
			      root->filename);
		if ((int)floor(y.value.d) >= x->dimensiune)
			error(ERROR_INDEXING_EXCEED_DIMENSIONS, root->linia, root->filename);
		x = &(((struct variable *)(x->value.v))[(int)floor(y.value.d)]);
	}
	return x;
}
