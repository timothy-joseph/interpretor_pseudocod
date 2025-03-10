#define ERROR_ALLOC_TOKENS "eroare la allocarea memoriei pentru tokenuri"
#define ERROR_REALLOC_TOKENS "eroare la reallocarea memoriei pentru tokenuri"
#define ERROR_ALLOC_NODE "erorare la allocarea memoriei pentru un nod nou la parser"
#define ERROR_UNKNOWN_CHAR_FORMAT "eroare: caracter necunoscut '%c' la linia %d in fisierul %s"
#define ERROR_NUMAR_DOTS_FORMAT "eroare: prea multe virgule la numar la linia %d in fisierul %s"
#define ERROR_CATTIMP_SYNTAX "syntaxa gresita pentru cat timp la linia %d in fisierul %s"
#define ERROR_PANACAND_SYNTAX "syntaxa gresita pentru pana cand la linia %d in fisierul %s"
#define ERROR_EXPERSSION_MALFORMED_FORMAT "syntaxa gresita a expresiei la linia %d in fisierul %s"
#define ERROR_PARANTHESIS_NOT_MATCHING_FORMAT "parantezele nu se potrivesc la linia %d in fisierul %s"
#define ERROR_INDENT_FORMAT "indentarea nu se potriveste la linia %d in fisierul %s"
#define ERROR_FARA_EXPRESIE_FORMAT "fara expresie la linia %d in fisierul %s"
#define ERROR_BEGIN_FORMAT "pentru instructiunea \"daca\", newline nu urmeaza imediat dupa begin sau altfel la linia %d in fisierul %s"
#define ERROR_MEMORIE_VARIABILA_FORMAT "nu s-a putut aloca destula memorie pentru o variabila noua declarata la linia %d in fisierul %s"
#define ERROR_MEMORIE_VARIABILA "nu s-a putut aloca destula memorie pentru o variabila noua"
#define ERROR_ASSIGN_FORMAT "syntaxa gresita pentru primire de valoare la linia %d in fisierul %s"
#define ERROR_VARIABLE_UNDECLARED_FORMAT "variabila %s este nedeclarata la linia %d in fisierul %s"
#define ERROR_REPETA_FORMAT "nu s-a gasit un \"cat timp\" sau un \"pana cand\" pentru instructiunea \"repeta\" care incepe la linia %d in fisierul %s"
#define ERROR_PENTRU_SEPEXP_FORMAT "prea multe sau prea putine expresii pentru instructiunea \"pentru\" la linia %d in fisierul %s"
#define ERROR_INDENT_SUBPROGRAM_FORMAT "subprogram definit in bloc de cod la linia %d in fisierul %s"
#define ERROR_LPAR_SUBPROGRAM_FORMAT "prea multe '(' la definirea unui subprogram la linia %d in fisierul %s"
#define ERROR_RPAR_SUBPROGRAM_FORMAT "prea multe ')' la definirea unui subprogram la linia %d in fisierul %s"
#define ERROR_NUME_SUBPROGRAM "nu a fost dat un nume subprogramului de la linia %d in fisierul %s"
#define ERROR_MALLOC_SUBPROGRAM "nu a fost destula memorie pentru a aloca loc pentru subprogram"
#define ERROR_VARIABILE_SUBPROGRAM_FORMAT "variabilele pentru subprogramul de la linia %d din fisierul %s nu au fost definite corect"
#define ERROR_NEDEFINIT_SUBPROGRAM_FORMAT "apel la un subprogram nedefinit '%s' la linia %d in fisierul %s"
#define ERROR_STACK_INSERT "nu s-a putut aloca memorie pentru un apel la subprogram"
#define ERROR_NOT_NUMAR "operatia %d de la linia %d in fisierul %s se poate face doar cu numere sau variabile care se pot interpreta ca numere"
/************************************/
#define ERROR_DIMENSIUNE_TIP "variabile de tip diferit de TV_NUMAR la dimensiunea unui vector la linia %d in fisierul %s"
#define ERROR_INDEXING_EXCEED_DIMENSIONS "s-au intrecut dimenisiunea tabloului la indexare la linia %d in fisierul %s"
#define ERROR_TIP "prea multi parametri la functia \"tip\" apela la linia %d in fisierul %s"
#define ERROR_CITESTETOT_PARAMETRII "prea multi parametri pentru \"citestetot\" de la linia %d in fisierul %s"
#define ERROR_FLOOR "prea multi parametri pentru functia \"floor\" de la linia %d in fisierul %s"
#define ERROR_SQRT "prea multi parametri la functia \"sqrt\" de la linia %d in fisierul %s"
#define ERROR_INDEXING_NON_VECTOR "variabila %s este inindexabila la linia %d in fisierul %s"
#define ERROR_INDEXING_NOT_NUMAR "indexarea sa face doar cu numere la linia %d in fisierul %s"
#define ERROR_FLOOR_NOT_NUMAR "paramentrul functiei \"floor\" trebuie sa fie de tip numar la linia %d in fisierul %s"
#define ERROR_SQRT_NOT_NUMAR "paramentrul functiei \"sqrt\" trebuie sa fie de tip TV_NUMAR la linia %d in fisierul %s"
#define ERROR_SQRT_BELOW_ZERO "parametrul functiei \"sqrt\" trebuie sa fie un numar mai mare sau egal ca zero la linia %d in fisierul %s"
#define ERROR_PREA_MULTE_ARGUMENT_DECLARARE_VECTOR "functia predefinita \"vector\" la linia %d din fisierul %s poate declara doar un vector unidimensional, iar tu ai specificat mai multe dimensiuni. daca voiai sa declari un vector prin valori trebuie sa scrii doar, de exemplu, (1, 2, 3, 4)"
#define ERROR_NOT_NUMAR_DECLARARE_VECTOR "functia predefinita \"vector\" la linia %d  in fisierul %s are ca parametru un numar"
#define ERROR_DIMENSIUNE "prea multi parametri functia \"dimensiune\" de la linia %d in fisierul %s"
#define ERROR_DIMENSIUNE_NOT_VECTOR "paramentrul functiei \"dimensiune\" trebuie sa fie de tip vector la linia %d in fisierul %s"
#define ERROR_CHAR_LEX_UNDEFINED_WELL "caracter nedefinit bine la linia %d in fisierul %s"
#define ERROR_STRING_NOT_FINISHED "sir de caractere neterminat la linia %d in fisierul %s"
#define ERROR_SCRIE_VECTOR "nu se poate scrie un vector normal folosind functia \"scrie\", se poate scrie doar un vector de caractere la linia %d in fisierul %s"
#define ERROR_TIP_FUNCTION "prea multi parametri la functia \"tip\" la linia %d in fisierul %s"
#define ERROR_CAST_FUNCTION "prea putini sau prea multi parametri (trebuie 2 parametrii anume varibila si tipul) la functia de cast la linia %d in fisierul %s"
#define ERROR_PREA_MULTE_ARGUMENTE_REFERENCE "prea multi parametrii la functia \"cast\" la linia %d in fisierul %s"
#define ERROR_REFERENCE_NOT_VAR "parametrul la functia referinta trebuie sa fie o variabila la linia %d in fisierul %s"
#define ERROR_VECTOR_NO_SPACE "nu s-a putut aloca destula memorie pentru vector la linia %d in fisierul %s"
#define ERROR_USER_DEFINED_FUNCTION_PARAMETERS "numar incorect de parametrii dati la subprogramul definit de utilizator %s la linia %d in fisierul %s"
#define ERROR_INDEXING_PARSE_NO_INDEX "trebuie dat un index la linia %d in fisierul %s"
#define ERROR_EXCEEDE_T "in etapa %s s-a intrecut peste constanta %d la linia %d in fisierul %s"
#define ERROR_CAST_TIP "trebuie dat un tip ca al doilea parametru la functia de schimbare tip la linia %d in fisierul %s"
#define ERROR_CAST_INVALID_TYPE "trebuie dat un tip valid (TV_CHAR, TV_NUMAR, TV_TIP; TV_VECTOR fiind nesecurizat) ca al doilea parametru la schimbare de tip la linia %d in fisierul %s"
#define ERROR_INSTRUCTION_SAME_LINE "la instructiunea \"daca\"/\"cat timp\"/\"pana cand\"/\"repeta\"/\"pentru\"/declarare subprogram de la linia %d in fisierul %s se pune pe linia urmatoare indentata cu un nivel in plus fata de instructiunea de decizie/repetitie/declarare subprogram (indentarea se face cu caracterul '|') instructiunea din interiorul structurii"
#define ERROR_PRESIDENCY_NUMBER 999

static void error(const char *s, ...);
static void free_on_quit(void);

static void
error(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	free_on_quit();
	exit(-1);
}
