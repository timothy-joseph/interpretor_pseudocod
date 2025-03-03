# Sah
-----

Am implementat o versiune incompleta a jocului de sah in pseudocod folosind citirea si afisarea in consola. Tabla de sah este memorata intr-un vector/tablou bidimensional.
Acesta are aproape toate miscarile, cu exceptia miscarii "en passant". Acesta poate detecta egalitate doar in cazurile stalemate, si 50 move rule, dar nu si in cazurile dead position rule, sau threefold repetition. In rest, jocul este complet.
Programul va detecta automat sah-ul si sahmat-ul.

---

Miscariile se scriu la promptul `mutare>`.
Miscariile nu se specifica folosind notatia de sah, ci se specifica casuta de inceput a piesei si unde trebuie mutata folosind sintaxa litera-numar (coloana-linie; exemplu: `D2 D4`).
Programul va detecta automat daca mutarea este valida.
In cazul in care o miscare este invalida, programul v-a explica criteriul folosit pentru a detecta ca miscarea este invalida. Tabla de sah este afisata la fiecare tura din perspectiva persoanei care trebuie sa mute.
