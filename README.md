Inainte de main:

Am utilizat trei structuri: TFis, TDir si TLG. TFis reprezinta arborele binar
de cautare format din fisiere. TDir este un arbore binar de cautare pentru
directoare, iar TLG reprezinta o lista simplu inlantuita ce contine cuvinte,
utilizata in special pentru a afisa caile catre directoare sau fisiere.

Am scris mai multe functii pentru eliberarea memoriei: distruge_fis, distruge_dir, 
Distruge_Arb si Distruge_Lista. Primele trei parcurg arborele recursiv si
dezaloca memoria. Ultima parcurge o lista printr-un for si elibereaza fiecare celula.

Pentru rezolvarea taskurilor am definit functiile: Constr_Fis, Constr_Dir,
Constr_Lista, Inserare_Dir, Inserare_Fis, Inserare_Lista, Selectare_Director,
Selectare_Fisier, cmp_sir, Cautare_Dir, Cautare_Dir2, Cautare_Fis, Afisare_Cale,
Afisare_Fis, Afisare_Dir, valoare_minima_dir, valoare_minima_fis, Sterge_Dir
si Sterge_Fis.

Constr_Fis aloca un nod al arborelui de fisiere in felul urmator: primeste ca parametru
un void * caruia ii copiaza toti bitii prin functia strdup si seteaza restul
campurilor drept NULL. Constr_Dir si Constr_Lista functioneaza similar numai ca
prima aloca un nod din arborele de directoare, iar a doua aloca o celula din lista.

Inserare_Dir creaza arborele de cautare, alocand nodul pentru radacina atunci 
cand aceasta e NULL. Dupa creaza un subdirector cand un director nu mai
are subdirectoare si atunci cand deja are subdirectoare,   parcurge arborele
de cautare si insereaza lexicografic in stanga sau dreapta.
Inserare_Fis functioneaza similar cu Inserare_Dir, doar ca aceasta introduce
informatii noi in arborele fisierelor.
Inserare_Lista parcurge o lista simplu inlantuita si insereaza cuvinte ordonat,
lexicografic.
Selectare_Director parcurge un subdirector cu scopul de a returna prin efect lateral
directorul care contine o anumita informatie.
Selectare_Fiser este asemanatoare cu functia anterioara, numai ca aceasta
returneaza prin efect lateral arborele de fisiere.
Functia cmp_sir casteaza doi void * si returneaza valoarea functiei strcmp.
Cautare_Dir parcurge intreg arborele de cautare incepand cu directorul curent, apeland
functia Selectare_Director ce cauta in subdirectoare si returnand valoarea lui *cont.
Atunci cand valoarea acestuia este 0, inseamna ca nu a fost gasit elementul respectiv,
iar 1 pentru cazul in care e gasit.
Cautare_Dir2 functioneaza similar cu functia anterioara doar ca nu se uita in 
subdirectoarele directorului curent.
Cautare_Fis este asemanatoare cu Cautare_Dir, doar ca este adaptata pentru arborele
de fisiere.
Afisare_Cale parcurge stramosii unui nod printr-un for, introducand la inceputul
unei liste numele acestora. Astfel se creaza calea cunoscuta in Linux drept "pwd",
pe care o printeaza si dupa ii elibereaza memoria.
Afisare_Dir parcurge elementele unui arbore de directoare si introduce ordonat numele
acestora intr-o lista (prin functia Inserare_Lista), ce este returnata prin efect
lateral.
Afisare_Fis este asemanatoare cu Afisare_Dir, doar ca este adaptata pentru arborele
de fisiere.
Functiile valoare_minima_dir si valoare_minima_fis parcurg nodurile din stanga ale
unui arbore de cautare, acolo unde se afla informatiile mai mici dpdv lexicografic.
Sterge_Dir si Sterge_Fis sterg un director sau fisier curent din restul arborelui,
apeland functiile explicate anterior si eliberand toata memoria.

In main:

Am alocat dinamic un char* pentru a putea citi linie cu linie de la tastatura.
Aceasta linie este fragmentata in mai multe cuvinte prin functia strtok, 
cuvinte ce reprezinta comenzile ce trebuie executate prin intermediul functiilor
explicate mai sus. La sfarsit se elibereaza restul memoriei ocupate de arbore prin DistrArb si de char*.