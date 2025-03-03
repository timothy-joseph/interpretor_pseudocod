.nr QI 10n
.nr HM 0.5i
.nr FM 0.5i
.ds TOC Cuprins
.RP no
.P1
.TL
Proiect de atestat - Interpretor de Pseudocod
.AU
Realizator: C\[u0069_0302]cu Timotei-Iosif
.ND ""
.AB no
Detalii de implementare a interpretorului de pseudocod, detalii despre utilizarea interpretorului, si exemple de pseudocod valide.
.AE
.NH
Introducere
.XS
Introducere
.XE
.PP
Am ales sa realizes acest proiect, deoarece sunt interesat in creerea limbajelor de programare compilate si interpretate.
.PP
Interpretorul a fost creat cu scopul de a invata despre limbajele de programare interpretate.
\
\
.NH
Prezentarea implementarii interpretorului de pseudocod
.XS
Prezentarea implementarii interpretorului de pseudocod
.XE
.PP
Am realizat interpretorul folosind in limbajul de programare C. Acesta are patru etape:
.NH 2
Etapa de lexare/tokenizare
.XS
	Etapa de lexare/tokenizare
.XE
.PP
Interpretorul citeste fisierul de intrare, apoi parcurge caracter cu caracter codul citit, transformandu-l intr-o serie de tokenuri care vor fi utilizate in etapa urmatoare (etapa de parsare).
\

.NH 2
Etapa de parsare
.XS
	Etapa de parsare
.XE
.PP
Programul parcurge tokenurile obtinute din etapa anterioara si formeaza un arbore de executare care este utilizat la etapare urmatoare.
.PP
Expresiile aritmetice sunt transformate din scrierea infix, in scriere prefix.
.PP
Exemplu de parte de arbore de executare:
.PSPIC arbore.ps
\

.NH 2
Etapa de executare
.XS
	Etapa de executare
.XE
.PP
Programul parcurge arborele de executare pentru a executa instructiuniile scrise in fisierul de intrare.
.SH 3
Apeluri de functii
.XS
		Apeluri de functii
.XE
.PP
Atunci cand o functie este apelata, se copiaza intr-un hash map de variabile paramentrii functiei, iar acest hash map este pus (push) intr-un stack (stiva).
.PP
Atunci cand o functie returneaza sau se termina, se scoate (pop) din stack (stiva), memoria alocata pentru hash mapul nou de variabile este eliberata si memoria alocata pentru noul element in stack (stiva) este si ea eliberata.
.SH 3
Variabile
.XS
		Variabile
.XE
.PP
Variabilele locale sunt memorate in hash mapul de variabile asociat capului stivei, iar variabilele globale sunt memorate in hash mapul
.CW global_var_map .
.PP
Atunci cand nu s-a apelat nicio functie, hash mapul de variabile locale din stack (stiva) este acelasi cu hash mapul de variable globale
.CW global_var_map .
\

.NH 2
Etapa de garbage collection:
.XS
	Etapa de garbage collection:
.XE
.PP
Variabilele de tip vector si de tip functie sunt alocate dinamic, iar atunci cand o variabila isi schimba valoarea si nu mai are referinta catre inceputul vectorului/catre functie, se poate creea un "memory leak".
.PP
Atunci cand un vector sau o functie sunt create, adresa lor de memorie este inserata intr-un hash map pentru a putea tine minte locatia lor.
.PP
Atunci cand o variabila primeste prin assignment, citire, sau apelare la functie o adresa de memorie care a fost alocata dinamic, hash mapul este actualizat si frecventa de cate variabile au referinta catre acea adresa creste.
.PP
Atunci cand o variabila care retinea o asemena adresa primeste prin assignment sau citire o noua valoare, sau se returneaza din functie si se elibereaza hash mapul de variabile locale, numarul de referinta catre acea parte de memorie scade.
.PP
Pointerele returnate de la functia
.CW referinta(x) ,
nu sunt contorizate.
.PP
Hash mapul utilizat pentru garbage collection este un hash map de frecventa
.PP
Atunci cand numarul de referinte a scazut pana la 0, se verifica cate alte adrese au un numar de referinte egal cu 0 si aceste adrese se elibereaza.
.PP
Exemple de cod in care s-ar elibera memorie folosind garbage collection
.DS I
.B1
.CW
x <- vector(3)
x <- 10
.B2
.DE
.PP
sau
.DS I
.B1
.CW
x <- vector(3)
citeste x
.B2
.DE
.PP
sau
.DS I
.B1
.CW
subprogramul f() face
|	x <- vector(3)
f()
.B2
.DE

.SH 2
Implementarea hash mapului
.XS
	Implementarea hash mapului
.XE
.PP
Si variable_map si possible_garbage_map folosesc aceeasi structura de date generalizata, folosind functii pentru utilitatiile speciale (o functie de hash, de indexare, de dereferentiere o singura data, de inserare in lista inlantuita, si de cautare in lista inlantuita)


.NH
Compilare si utilizare a interpretorului
.XS
Compilare si utilizare a interpretorului
.XE
.NH 2
Compilare
.XS
	Compilare
.XE
.PP
Interpretorul se complileaza utilizand compilatorul 
.CW gcc 
(sau echivalent) folosind urmatoarea comanda:
.CW "gcc main.c -lm -o pseudocod"
\

.NH 2
Utilizare
.XS
	Utilizare
.XE
.PP
Interpretorul se apeleaza folosind comanda
.CW "pseudocod [fisier]"
- unde fisier este fisierul care contine codul sursa


.NH
Sintaxa utilizata
.XS
Sintaxa utilizata
.XE
.NH 2
Indentare
.XS
	Indentare
.XE
.PP
Indentarea sa face folosind caracterul 
.CW | .
.PP
Exemplu:
.DS I
.B1
.CW
daca 1 atunci
|	scrie "da"
.B2
.DE
.PP
Daca la sfarsitul liniei anterioare s-a folosit caracterul de escape 
.CW \\\\ ,
atunci nu se va mai indenta folosind, deoarece interpretorul trateaza acest caz ca si cand nu ar fi o linie noua intre cele doua linii
.PP
Exemplu:
.DS I
.B1
.CW
daca 1 si \\
     2 atunci
|	scrie "da"
.B2
.DE
.PP
Instructiuniile de decizie, repetitive, si definirea functiilor au nevoie de indentare pentru a determina codul din interiorul structurii.
\

.NH 2
Expresiile (instructiunile aritmetice)
.XS
	Expresiile (instructiunile aritmetice)
.XE
.PP
Expresiile sunt combinari valide de operatori unari si binari, variabile, valori, si apeluri la functii
.PP
Operaturul unar este
.CW not ", iar operatorii binari sunt: "
.CW <- 
(assignment),
.CW sau ,
.CW si ,
.CW = ,
.CW != ,
.CW < ,
.CW > ,
.CW <= ,
.CW >= ,
.CW + ,
.CW - ,
.CW * ,
.CW / ,
.CW % 
(restul impartirii),
.CW ^ 
(ridicarea la putere).
.SH 3
Ordinea operatorilor
.XS
		Ordinea operatorilor
.XE
.IP \(bu
Ordinul 9:
.CW not
.IP \(bu
Ordinul 8:
.CW ^
.IP \(bu
Ordinul 7:
.CW * ,
.CW / ,
.CW % 
(restul impartirii)
.IP \(bu
Ordinul 5:
.CW + ,
.CW -
.IP \(bu
Ordinul 4:
.CW < ,
.CW > ,
.CW <= ,
.CW >=
.IP \(bu
Ordinul 3:
.CW = ,
.CW !=
.IP \(bu
Ordinul 2:
.CW si
.IP \(bu
Ordinul 1:
.CW sau
.IP \(bu
Ordinul 0:
.CW <- 
(assignment)
.PP
Operatiile de ordin mai mare vor fi executate primele.
.PP
Exemple de expresii valide:
.DS I
.B1
.CW
x <- 10 * 2 / 2 * f(x)
.B2
.DE
.DS I
.B1
.CW
x = x != x
.B2
.DE
.DS I
.B1
.CW
x <- -1
.B2
.DE
.PP
Exemple de expresii invalide
.DS I
.B1
.CW
x <- 10 * 2 / 2 *
.B2
.DE
.DS I
.B1
.CW
x = != x
.B2
.DE
.DS I
.B1
.CW
x <- * 1
.B2
.DE
\

.NH 2
Variabilele
.XS
	Variabilele
.XE
.PP
Variabilele sunt declarate prin asignare, citire, sau prin definirea unor functii (functiile definite de utilizator sunt variabile).
.PP
Numele variabilelor pot sa contina litere, caracterul.
.CW _ 
(underscore), si numere, dar nu pot sa inceapa cu numere.

.NH 2
Tipurile de variabile
.XS
	Tipurile de variabile
.XE
.PP
Tipurile de variabile sunt:
.CW TV_NUMAR 
(numere de tip intreg si real),
.CW TV_CHAR 
(caractere),
.CW TV_VECTOR 
(vectori de alte tipuri de variabile),
.CW TV_TIP 
(returnat de functia 
.CW tip() ),
.CW TV_ERROR 
(returnat de functia 
.CW tip() ).
.PP
Prescurtarea
.CW TV
vine de la "tip variabila".
.PP
Functia 
.CW tip() 
este explicata la sectiunea despre functiile predefinite.
\

.NH 2
Declararea si apelarea functiilor
.XS
	Declararea si apelarea functiilor
.XE
.PP
Functiile sunt definite utilizand cuvantul cheie
.CW subprogramul ", urmat de numele subprogramul, lista de parametrii in paranteze, si cuvantul cheie "
.CW face 
astfel:
.DS I
.B1
.CW
subprogramul nume_subprogram(p0, p1, p2, ...) face
|	cod
.B2
.DE
.PP
Functiile sunt apelate folosind numele subprogramul si lista de variabile sau valori in paranteze astfel:
.DS I
.B1
.CW
nume_subprogram(v0, v1, v2, ...)
.B2
.DE
.PP
Daca functia nu are parametrii, atunci se va declara astfel:
.DS I
.B1
.CW
subprogramul nume_subprogram() face
|	cod
.B2
.DE
.PP
Si se va apela astfel:
.DS I
.B1
.CW
nume_subprogram()
.B2
.DE
.NH 3
Functii predefinite
.XS
		Functii predefinite
.XE
.IP \(bu
.CW sqrt(x) 
- radical din x
.IP \(bu
.CW floor(x) 
- da partea intreaga a lui x
.IP \(bu
.CW tip(x) 
- returneaza tipul lui x, valoarea returnata are tip 
.CW TV_TIP
.IP \(bu
.CW vector(x) 
- alocheaza memorie pentru a creea un vector de dimenisiune x
.IP \(bu
.CW dimensiune(x) 
- da dimensiunea vectorului x
.IP \(bu
.CW referinta(x) 
- returneaza o referinta catre x sub forma unui vector cu dimensiune 1, functie folosita pentru a modifica valoarea unei variabile neglobale in interiorul unui subprogram. se derefentiaza prin accesarea indexului 0
.IP \(bu
.CW "cast(x, tip) si schimbare_tip(x, tip)" 
- returneaza valoarea lui x, dar cu tip 
.CW tip ", "
.CW tip 
este o variabila sau o constanta de tip 
.CW TV_TIP
\

.NH 2
Instructiunea de decizie
.XS
	Instructiunea de decizie
.XE
.PP
Instructiunea de decizie se foloseste astfel:
.DS I
.B1
.CW
daca expr0 atunci
|	cod daca expr0 este adevarat
.B2
.DE
.PP
Instructiunea de decizie cu doua ramuri:
.DS I
.B1
.CW
daca expr0 atunci
|	cod daca expr0 este adevarat
|altfel
|	cod daca expr0 este fals
.B2
.DE
.PP
Instructiunea de decizie cu mai mult de doua ramuri:
.DS I
.B1
.CW
daca expr0 atunci
|       cod daca expr0 este adevarat
|altfel daca expr1 atunci
|       |	cod daca expr0 este fals si expr1 este adevarat
|       |altfel
|       |	cod daca expr0 este fals si expr1 este fals
.B2
.DE
.PP
.B "Observatie:" " Spre deosebire de celelalte structuri dupa care trebuie sa urmeze indentatie, dupa cuvantul cheie "
.CW altfel 
poate urma o expresie/instructiune direct.
\

.NH 2
Instructiuni repetitive cu numar nedeterminat de pasi
.XS
	Instructiuni repetitive cu numar nedeterminat de pasi
.XE
\
.NH 3
Instructiuni repetitive cu test initial
.XS
		Instructiuni repetitive cu test initial
.XE
.PP
Instructiunile repetitive cu test initial vor executa codul din interiorul structurii doar daca expresia este adevarata.
.NH 4
Instructiunea cat timp ... executa
.XS
			Instructiunea cat timp ... executa
.XE
.PP
Instructiunea
.CW "cat timp ... executa" 
se foloseste astfel:
.DS I
.B1
.CW
cat timp expr0 executa
|	cod daca expr0 este adevarat
.B2
.DE
\
.NH 4
Instructiunea pana cand ... executa
.XS
			Instructiunea pana cand ... executa
.XE
.PP
Instructiunea
.CW "pana cand ... executa" 
se foloseste astfel:
.DS I
.B1
.CW
pana cand expr0 executa
|	cod daca expr0 este fals
.B2
.DE
\
.NH 3
Instructiuni repetitive cu test final
.XS
		Instructiuni repetitive cu test final
.XE
.PP
Instructiunile repetitive cu test final vor executa cel putin o data codul din structura, indiferent daca expresia este adevarata, apoi vor executa codul din nou, doar daca expresia este adevarata
.NH 4
Instructiunea repeta ... cat timp
.XS
			Instructiunea repeta ... cat timp
.XE
.PP
Instructiunea
.CW "repeta ... cat timp" 
se foloseste astfel:
.DS I
.B1
.CW
repeta
|	cod daca expr0 este adevarat
|cat timp expr0
.B2
.DE
\
.NH 4
Instructiunea repeta ... pana cand
.XS
			Instructiunea repeta ... pana cand
.XE
.PP
Instructiunea
.CW "repeta ... pana cand" 
se foloseste astfel:
.DS I
.B1
.CW
repeta
|	cod daca expr0 este fals
|pana cand expr0
.B2
.DE
\

.NH 2
Instructiunea repetitiva cu numar determinat de pasi
.XS
	Instructiunea repetitiva cu numar determinat de pasi
.XE
.NH 3
Instructiunea pentru ... executa
.XS
		Instructiunea pentru ... executa
.XE
.PP
Instructiunea
.CW "pentru ... executa" 
se foloseste astfel:
.DS I
.B1
.CW
pentru expr0, expr1, expr2 executa
|	cod daca expr1 este adevarata
.B2
.DE
.PP
.CW expr0 
va fi executata, apoi se va testa daca 
.CW expr1 
este adevarata si daca este adevarata, se va executa codul din structura, la sfarsitul codului din structura, se va executa 
.CW expr2 
si se va intoarce la testare.
.PP
.B Observatie: 
spre deosebire de pseudocodul invatat la scoala, aceasta instructiune are aceeasi functionalitate ca si un 
.CW for 
din C.
\

.NH 2
Instructiuni de intrare/iesire
.XS
	Instructiuni de intrare/iesire
.XE
.NH 3
Instructiuni de intrare
.XS
		Instructiuni de intrare
.XE
.NH 4
Instructiunea citestetot
.XS
			Instructiunea citestetot
.XE
.PP
Instructiunea 
.CW citestetot 
va fi urmata de o variabila si se va citi de la tastatura un sir de caractere pana la caracterul de linie noua si se va stoca in variabila sub forma unui vector de caractere.
.PP
Exemplu:
.DS I
.B1
.CW
citestetot x
.B2
.DE
\
.NH 4
Instructiunea citeste
.XS
			Instructiunea citeste
.XE
.PP
Instructiunea 
.CW citeste 
va fi urmata de un numar nedeterminat de variabile si se va citi de la tastatura cuvant cu cuvant si se va determina daca poate fi interpretat ca numar. Daca poate fi interpretat ca numar, atunci va fi memorat ca numar (
.CW TV_NUMAR 
) in variabila corespunzatoare numarului cuvantului. Daca nu poate fi interpretat ca numar, atunci se va stoca in variabila corespunzatoare ca un vector de caractere.
.PP
Exemplu:
.DS I
.B1
.CW
citeste x, y, z
.B2
.DE
.PP
Si se citeste de la tastatura:
.DS I
.B1
.CW
abc 123 b1
.B2
.DE
.PP
Atunci 
.CW x 
va memora vectorul de caratere \*Qabc\*U, 
.CW y 
va memora numarul 123, 
.CW z 
va memora vectorul de caratere \*Qb1\*U.
.PP
Exemplu:
.DS I
.B1
.CW
citeste x
.B2
.DE
.PP
Si se citeste de la tastatura:
.DS I
.B1
.CW
12.12
.B2
.DE
.PP
Atunci 
.CW x 
va memora numarul 12.12 (doisprezece virgula doisprezece).
\
\
.NH 3
Instructiuni de iesire
.XS
		Instructiuni de iesire
.XE
.NH 4
Instructiunea scrie
.XS
			Instructiunea scrie
.XE
.PP
Instructiunea 
.CW scrie 
va fi urmata de un numar nedeterminat de variabile si valori care vor fi afisate in consola in functie de tipul lor:
.IP \(bu
.CW TV_NUMAR 
- va afisa numarul
.IP \(bu
.CW TV_CHAR 
- va afisa caracterul
.IP \(bu
.CW TV_VECTOR 
- daca este un sir de caractere, atunci se va afisa sirul de caractere. Daca nu este sir de caractere, atunci interpretorul va da eroare
.IP \(bu
.CW TV_TIP 
- va afisa 
.CW TV_NUMAR 
, 
.CW TV_CHAR 
, 
.CW TV_VECTOR 
, 
.CW TV_TIP 
, sau 
.CW TV_ERROR 
in functie de valoarea variabilei
.IP \(bu
.CW TV_ERROR 
- se va trata precum variabila are tip de numar
.CW TV_NUMAR ) (
\

.NH 2
Vectori
.XS
	Vectori
.XE
.NH 3
Declarea vectoriilor
.XS
		Declarea vectoriilor
.XE
.PP
Vectorii pot fi declarati in doua moduri.
.DS I
.B1
.CW
x <- vector(3)
.B2
.DE
.PP
care va creea un vector de dimensiune 3
.PP
si
.DS I
.B1
.CW
x <- (1, 2, 3)
.B2
.DE
.PP
care va creea un vector de dimensiune 3 cu valoriile 1, 2, 3.
.PP
Vectorii pot avea mai multe dimesiuni astfel:
.DS I
.B1
.CW
x <- vector(3)
x[0] <- vector(3)
.B2
.DE
.PP
si
.DS I
.B1
.CW
x <- ((1, 2, 3), 2, 3)
.B2
.DE
\
.NH 3
Indexarea vectoriilor
.XS
		Indexarea vectoriilor
.XE
.PP
Vectorii pot fi indexati utilizand urmatoarea sintaxa:
.DS I
.B1
.CW
x[expr0][expr1]...[exprn]
.B2
.DE
.PP
Exemplu:
.DS I
.B1
.CW
x <- ((1, 2, 3), 2, 3)
scrie x[0][1], " ", x[2]
.B2
.DE
.PP
va afisa:
.DS I
.B1
.CW
2 3
.B2
.DE
\

.NH 2
Cuvatul cheie sursa
.XS
	Cuvatul cheie sursa
.XE
.PP
Acest cuvant cheie este singurul care este interpretat in etapa de lexing, similar cu C. Codul din noul fisier este inserat in lista de tokenuri exact in locul in care apare, iar mai tarziu este tratat la fel ca si codul din sursa principala.
.PP
Dupa cuvantul cheie sursa, se va scrie locatia fisierului care sa fie inclus.
.PP
Exemplu:
.PP
Fisierul
.CW main.pseudo
.DS I
.B1
.CW
sursa "alta_sursa.pseudo"
f()
.B2
.DE
.PP
Fisierul
.CW alta_sursa.pseudo
.DS I
.B1
.CW
subprogramul f() face
|	scrie "123"
.B2
.DE
.PP
Va afisa:
.DS I
.B1
.CW
123
.B2
.DE


.NH 2
Comentarii
.XS
	Comentarii
.XE
.PP
Comentariile pot fi scrise utilizand caracterul
.CW # 
similar cu python. Tot ce urmeaza dupa caracterul
.CW # 
va fi ignorat pana la urmatoarea linie.
.PP
Exemplu:
.DS I
.B1
.CW
# daca 1 atunci
# |	scrie 2
.B2
.DE
.PP
Nu va afisa nimic.


.NH
Exemple de cod in directiva "examples"
.XS
Exemple de cod in directiva "examples"
.XE
.PP
Am realizat cateva programe folosind pseudocodul/limbajul de programare inventat de mine pentru a afisa abilitatiile acestuia.
\

.NH 2
X si 0
.XS
	X si 0
.XE
.PP
Am implementat jocul de x si 0 in pseudocod folosind citirea si afisarea in consola. Tabla de joc este memorata intr-un vector/tablou bidimensional.
.PP
Atunci cand trebuie intrata casuta in care se va pune simbolul (x sau 0), se va afisa un prompt care evidentiaza a cui tura este. Inputul este reprezentat de un numar intre 1 si 3 pentru linie si un numar intre 1 si 3 pentru coloana, acestea fiind separate printr-un spatiu.
.PP
Programul va determina automat cazurile de castig sau egalitate.
\

.NH 2
Sah
.XS
	Sah
.XE
.PP
Am implementat o versiune incompleta a jocului de sah in pseudocod folosind citirea si afisarea in consola. Tabla de sah este memorata intr-un vector/tablou bidimensional.
.PP
Acesta are aproape toate miscarile, cu exceptia miscarii "en passant". Acesta poate detecta egalitate doar in cazurile stalemate, si 50 move rule, dar nu si in cazurile dead position rule, sau threefold repetition. In rest, jocul este complet.
.PP
Programul va detecta automat sah-ul si sahmat-ul.
.PP
Miscariile se scriu la promptul
.CW mutare> .
Miscariile nu se specifica folosind notatia de sah, ci se specifica casuta de inceput a piesei si unde trebuie mutata folosind sintaxa litera-numar (coloana-linie; exemplu:
.CW "D2 D4" ).
Programul va detecta automat daca mutarea este valida.
.PP
In cazul in care o miscare este invalida, programul v-a explica criteriul folosit pentru a detecta ca miscarea este invalida. Tabla de sah este afisata la fiecare tura din perspectiva persoanei care trebuie sa mute.
\

.NH 2
Probleme de eficienta
.XS
	Probleme de eficienta
.XE
.PP
Am rezolvat o parte (10 probleme) din problemele propuse la proba practica pentru examenul de atestare a competentelor profesionale a absolventilor claselor de matematica-informatica si matematica-informatica, intensiv informatica, in pseudocod.
.PP
Deoarece nu am implementat citire si afisare in fisier in pseudocod, am modificat cerinta unor probleme, astfel incat sa foloseasca citirea de la tastatura si afisarea in consola (stdin si stdout).


.NH
Pentru utilizatorii de linux
.XS
Pentru utilizatorii de linux
.XE
.PP
Programul se poate instala folosind comanda
.CW "sudo make install"


.NH
Pentru utilizatorii de vim
.XS
Pentru utilizatorii de vim
.XE
.PP
In folderul/directiva
.CW vim
am inclus fisierul
.CW pseudo.vim
pentru sintaxa astfel incat vim sa foloseasca highlightarea de sintaxa pentru pseudocod, de exemplu:
.PSPIC vim.ps
.PP
Fisierul trebuie plasat in
.CW ~/.vim/syntax
sau in alt folder echivalent.


.bp
.XS
Cuprins
.XE
.PX
