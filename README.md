<b>#Rosu Mihai Cosmin 313CA</b>

- Folosesc o bucla infinita pentru a citii comenzile, dupa care identific tipul
acestora.

<b>LOAD:</b>
- Incerc sa deschid fisierul cu numele introdus. Daca reusesc incep sa preiau
datele despre imagine.
- Pentru fiecare imagine retin tipul, dimensiunile, valoarea maxima a pixelilor
si valorile pixelilor intr-o matrice.
- Pentru imaginile color folosesc structura rgb, iar pentru imaginile grayscale
si alb-negru folosesc tipul de date short.
- Inainte sa preiau valorile pixelilor, verific daca imaginea este in format
binar sau text, in functie de tipul acesteia.
- Daca este text continui citirea, altfel redeschid fisierul in mod binar si
sar peste informatia deja citita.
- In cazul in care mai aveam o imagine incarcata in program, o eliberez mai
intai, dupa care aloc din nou memorie pentru noua imagine.
- Dupa incarcarea imaginii, selectez intreaga ei suprafata.

<b>SELECT:</b>
- Preiau parametrii comenzii si verific daca sunt in numar necesar si pentru
fiecare parametru in parte daca este numar.
- Daca este cazul ca punctele primite sa nu aiba coordonatele in ordine
crescatoare le interschimb.
- Inainte sa fac selectia verific daca toate coordonatele sunt valide si nu
se afla in afara imaginii.
- Daca toate coordonatele sunt corecte selectez suprafata delimitata de
acestea.

<b>SELECT ALL:</b>
- Verific daca dupa ce am primit comanda SELECT se afla un singur parametru
si anume ALL, caz in care selectez intreaga suprafata a imaginii.

<b>CROP:</b>
- Pentru CROP folosesc doua functii, una pentru imaginiile grayscale si una
pentru imaginile color.
- In ambele cazuri copiez selectia in coltul din stanga sus al matricei
pixelilor si modific dimensiunile corespunzator.

<b>ROTATE:</b>
- Pentru ROTATE folosesc patru functii. Doua sunt pentru rotirea unei selectii,
care poate avea loc doar daca selectia este patratica, si fiecare functie este
pentru un tip de imagine: grayscale si color. Celelalte doua functii sunt
pentru rotirea intregii imagini, caz in care nu trebuie sa fie patratica si
poate necesita realocarea memoriei.
- In toate cazurile verific daca unghiul pentru rotire are valoare corecta.
- Pentru rotirea selectiei verific daca are forma patratica.
- Apoi folosesc o copie a acesteia pentru a o roti mai usor dupa care trec noile
valori inapoi in matricea originala.
- Pentru rotirea intregii imagini fac o copie a matricei originale.
- Apoi preiau unghiul de rotire si verific daca este egal cu 180 sau -180, caz
in care nu este necesara realocarea memoriei, altfel realoc matricea cu
dimensiunile inversate.
- Dupa aceea rotesc copia in-place sau in timp ce ii trec valorile in matricea
originala.
- In final actualizez dimensiunile.

<b>SAVE:</b>
- Preiau numele fisierului si apoi verific daca exista parametrul optional.
- In cazul in care exista, verific daca este cuvantul ascii, caz in care trebuie
sa salvez imaginea ca fisier text.
- In ambele cazuri mai intai trec in format text tipul imaginii, dimensiunile si
valoarea maxima a pixelilor.
- Apoi, daca trebuie salvata in mod text continui sa scriu valorile pixelilor
din matrice, altfel redeschid fisierul in modul "append" si in format binar,
dupa care incep sa scriu valorile pixelilor in mod binar.

<b>GRAYSCALE:</b>
- Verific daca imaginea este color, caz in care pentru fiecare pixel actualizez
cele trei valori ale culorilor cu media lor aritmetica.
- Dupa aceea aproximez fiecare valoare la cel mai apropiat intreg.

<b>SEPIA:</b>
- Verific daca imaginea este color, caz in care pentru fiecare pixel actualizez
cele trei valori dupa formula necesara filtrului sepia.
- Dupa aceea verific daca noile valori nu depasesc 255, si le aproximez la cel
mai apropiat intreg.

<b>EXIT:</b>
- Folosesc un "break" pentru a iesi din bucla infinita.

</br>
- Pentru fiecare comanda in afara de LOAD, daca nu era incarcata o imagine nu se
producea nicio modificare, si se afisa un mesaj corespunzator.</br>
- Orice alta comanda in afara celor de mai sus este considerata invalida.</br>
- In final eliberez memoria matricelor folosite.</br>
