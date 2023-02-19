/* LEPADATU Darius - 312 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINIE 100

typedef struct nod_fis {
    char *nume;
    struct nod_dir *parent;
	struct nod_fis *st, *dr;
} TNodFis, *TFis;

typedef struct nod_dir {
    char *nume;
    struct nod_fis *files;
    struct nod_dir *parent, *directories;
	struct nod_dir *st, *dr;
} TNodDir, *TDir;

typedef struct celulag {
    char *cuv;
    struct celulag *urm;
} TCelulaG, *TLG;

// Am scris mai multe functii pentru eliberarea memoriei: distruge_fis, distruge_dir, 
// Distruge_Arb si Distruge_Lista. Primele trei parcurg arborele recursiv si
// dezaloca memoria. Ultima parcurge o lista printr-un for si elibereaza fiecare celula.

void distruge_fis(TFis r) {
	if (!r) return;
    free(r->nume);
	distruge_fis(r->st);
	distruge_fis(r->dr);
	free(r);
}

void distruge_dir(TDir r) {
	if (!r) return;
    free(r->nume);
    distruge_fis(r->files);
    distruge_dir(r->directories);
	distruge_dir(r->st);
	distruge_dir(r->dr);
	free(r);
}

void Distruge_Arb(TDir *a) {
	if (!(*a)) return;
	distruge_dir((*a));
	*a = NULL;
}

void Distruge_Lista(TLG *aL) {
    TLG p = NULL, aux = NULL;
    for (p = *aL; p != NULL; ) {
        free(p->cuv);
        aux = p;
        p = p->urm;
        free(aux);
    }
    *aL = NULL;
}

// Constr_Fis aloca un nod al arborelui de fisiere in felul urmator: primeste ca parametru
// un void * caruia ii copiaza toti bitii prin functia strdup si seteaza restul
// campurilor drept NULL.

TFis Constr_Fis(void *x) {
	TFis aux = (TFis)calloc(1, sizeof(TNodFis));
	if (!aux) return NULL;
    aux->nume = strdup(x);
    aux->parent = NULL;
    aux->st = aux->dr = NULL;
	return aux;
}

// Constr_Dir si Constr_Lista functioneaza similar numai ca
// prima aloca un nod din arborele de directoare, iar a doua aloca o celula din lista.

TDir Constr_Dir(void *x) {
	TDir aux = (TDir)calloc(1, sizeof(TNodDir));
	if (!aux) return NULL;
    aux->nume = strdup(x);
    aux->parent = NULL;
    aux->directories = NULL;
    aux->files = NULL;
    aux->st = aux->dr = NULL;
	return aux;
}

TLG Constr_Lista(void *x) {
    TLG aux = (TLG)calloc(1, sizeof(TCelulaG));
    aux->cuv = strdup(x);
    aux->urm = NULL;
    return aux;
}

// Inserare_Dir creaza arborele de cautare, alocand nodul pentru radacina atunci 
// cand aceasta e NULL. Dupa creaza un subdirector cand un director nu mai
// are subdirectoare si atunci cand deja are subdirectoare,   parcurge arborele
// de cautare si insereaza lexicografic in stanga sau dreapta.

int Inserare_Dir(TDir *a, void *x, int(*cmp_sir)(void*, void*)) {
	TDir aux = NULL, p = NULL, n = NULL;
	if (!(*a)) {	
		*a = Constr_Dir(x);
		if (!*a) return 0;
		return 1;
	}
    if (!(*a)->directories) {
		aux = Constr_Dir(x);
        (*a)->directories = aux;
        aux->parent = (*a);
        return 1;
    }
	n = (*a)->directories;
	while (n) {
		p = n;
		if (cmp_sir(x, n->nume) == 0)  {
            printf("Directory %s already exists!\n", n->nume);
			return 0;
        } else if (cmp_sir(x, n->nume) < 0) {
			n = n->st;
        } else {
			n = n->dr;
        }	
	}
	aux = Constr_Dir(x);
    aux->parent = (*a);
	if (!aux) return 0;
	if (cmp_sir(x, p->nume) < 0) {
		p->st = aux;
    } else {
		p->dr = aux;
    }
	return 1;
}

// Inserare_Fis functioneaza similar cu Inserare_Dir, doar ca aceasta introduce
// informatii noi in arborele fisierelor.

int Inserare_Fis(TDir *a, void *x, int(*cmp_sir)(void*, void*)) {
	TFis aux = NULL, p = NULL, n = NULL;
	if (!(*a)) {	
		*a = Constr_Dir(x);
		if (!*a) return 0;
		return 1;
	}
    if (!(*a)->files) {
		aux = Constr_Fis(x);
        (*a)->files= aux;
        aux->parent = (*a);
        return 1;
    }
	n = (*a)->files;
	while (n) {
		p = n;
		if (cmp_sir(x, n->nume) == 0)  {
            printf("File %s already exists!\n", n->nume);
			return 0;
        } else if (cmp_sir(x, n->nume) < 0) {
			n = n->st;
        } else {
			n = n->dr;
        }	
	}
	aux = Constr_Fis(x);
    aux->parent = (*a);
	if (!aux) return 0;
	if (cmp_sir(x, p->nume) < 0) {
		p->st = aux;
    } else {
		p->dr = aux;
    }
	return 1;
}

// Inserare_Lista parcurge o lista simplu inlantuita si insereaza cuvinte ordonat,
// lexicografic.

int Inserare_Lista(TLG *L, void *cuv, int (*cmp_sir)(void *, void*)) {
    TLG p = NULL, aux = NULL, ant = NULL;
    for (p = *L; p != NULL ; p = p->urm) {
        if (cmp_sir(p->cuv, cuv) > 0) {
            break;
        }
        ant = p;
    }
    aux = Constr_Lista(cuv);
    if (!aux) return 0;
    if (!ant) {
        aux->urm = *L;
        *L = aux;
    } else {
        ant->urm = aux;
        aux->urm = p;
    }
    return 1;
}

// Selectare_Director parcurge un subdirector cu scopul de a returna prin efect lateral
// directorul care contine o anumita informatie.

int Selectare_Director(TDir a, void *x, int(*cmp_sir)(void *, void*), TDir *p) {
	if(!a) return 0;
    if (cmp_sir(a->nume, x) == 0) {
        *p = a;
        return 1;
    }
    if (Selectare_Director(a->st, x, cmp_sir, p)) return 1;
    if (Selectare_Director(a->dr, x, cmp_sir, p)) return 1;
    return 0;
}

// Selectare_Fiser este asemanatoare cu functia anterioara, numai ca aceasta
// returneaza prin efect lateral arborele de fisiere.

int Selectare_Fisier(TFis a, void *x, int(*cmp_sir)(void *, void*), TFis *p) {
	if(!a) return 0;
    if (cmp_sir(a->nume, x) == 0) {
        *p = a;
        return 1;
    }
    if (Selectare_Fisier(a->st, x, cmp_sir, p)) return 1;
    if (Selectare_Fisier(a->dr, x, cmp_sir, p)) return 1;
    return 0;
}

int cmp_sir(void *x, void *y) {
    char *a  = (char*)x, *b = (char*)y;
    return strcmp(a, b);
}

// Cautare_Dir parcurge intreg arborele de cautare incepand cu directorul curent, apeland
// functia Selectare_Director ce cauta in subdirectoare si returnand valoarea lui *cont.
// Atunci cand valoarea acestuia este 0, inseamna ca nu a fost gasit elementul respectiv,
// iar 1 pentru cazul in care e gasit.

int Cautare_Dir(TDir a, void *x, int *cont, TDir *p) {
	if(!a) return *cont;
	if(!a->st && !a->dr) {
        if (Selectare_Director(a, x, cmp_sir, p)) {
            *cont = 1;
            return *cont;
        }
    }
    if (Selectare_Director(a, x, cmp_sir, p)) {
        *cont = 1;
        return *cont;
    }
    Cautare_Dir(a->directories, x, cont, p);
	Cautare_Dir(a->st, x, cont, p);
	Cautare_Dir(a->dr, x, cont, p);
    return *cont; 
}

int Cautare_Dir2(TDir a, void *x, int *cont, TDir *p) {
	if(!a) return *cont;
	if(!a->st && !a->dr) {
        if (Selectare_Director(a, x, cmp_sir, p)) {
            *cont = 1;
            return *cont;
        }
    }
    if (Selectare_Director(a, x, cmp_sir, p)) {
        *cont = 1;
        return *cont;
    }
	Cautare_Dir2(a->st, x, cont, p);
	Cautare_Dir2(a->dr, x, cont, p);
    return *cont; 
}

// Cautare_Fis este asemanatoare cu Cautare_Dir, doar ca este adaptata pentru arborele
// de fisiere.

int Cautare_Fis(TDir a, void *x, int *cont, TDir *p) {
    TFis p2 = NULL;
	if(!a) return *cont;
	if(!a->st && !a->dr) {
        if (Selectare_Fisier(a->files, x, cmp_sir, &p2)) {
            *p = a;
            *cont = 1;
            return *cont;
        }
    }
    if (Selectare_Fisier(a->files, x, cmp_sir, &p2)) {
        *p = a;
        *cont = 1;
        return *cont;
    }
    Cautare_Fis(a->directories, x, cont, p);
	Cautare_Fis(a->st, x, cont, p);
	Cautare_Fis(a->dr, x, cont, p);
    return *cont; 
}

// Afisare_Cale parcurge stramosii unui nod printr-un for, introducand la inceputul
// unei liste numele acestora. Astfel se creaza calea cunoscuta in Linux drept "pwd",
// pe care o printeaza si dupa ii elibereaza memoria.

void Afisare_Cale(TDir a) {
    TLG L = NULL, aux = NULL, p = NULL;
    TDir n = NULL;
    if (!a) { 
        return;
    }
    for (n = a; n != NULL; n = n->parent) {
        aux = Constr_Lista(n->nume);
        aux->urm = L;
        L = aux;
    }
    for (p = L; p != NULL; p = p->urm) {
        printf("/%s", p->cuv);
    }
    if (L) printf("\n");
    Distruge_Lista(&L);
}

// Afisare_Dir parcurge elementele unui arbore de directoare si introduce ordonat numele
// acestora intr-o lista (prin functia Inserare_Lista), ce este returnata prin efect
// lateral.

void Afisare_Dir(TDir a, TLG *L) {
	if(!a) return;
	if(!a->st && !a->dr) {
        Inserare_Lista(L, a->nume, cmp_sir);
        return;
    }
    Inserare_Lista(L, a->nume, cmp_sir);
	Afisare_Dir(a->st, L);
	Afisare_Dir(a->dr, L);
}

// Afisare_Fis este asemanatoare cu Afisare_Dir, doar ca este adaptata pentru arborele
// de fisiere.

void Afisare_Fis(TFis a, TLG *L) {
	if(!a) return;
	if(!a->st && !a->dr) {
        Inserare_Lista(L, a->nume, cmp_sir);
        return;
    }
    Inserare_Lista(L, a->nume, cmp_sir);
	Afisare_Fis(a->st, L);
	Afisare_Fis(a->dr, L);
}

TDir valoare_minima_dir(TDir a) {
    TDir curent = a;
    while (curent && curent->st) {
        curent = curent->st;
    }
    return curent;
}

TFis valoare_minima_fis(TFis a) {
    TFis curent = a;
    while (curent && curent->st) {
        curent = curent->st;
    }
    return curent;
}

// Sterge_Dir si Sterge_Fis sterg un director sau fisier curent din restul arborelui,
// apeland functiile explicate anterior si eliberand toata memoria.

TDir Sterge_Dir(TDir a, char *nume) {
    TDir aux = NULL;
    if (!a) return NULL;;
    if (cmp_sir(nume, a->nume) < 0) {
        a->st = Sterge_Dir(a->st, nume);
    } else if (cmp_sir(nume, a->nume) > 0) {
        a->dr = Sterge_Dir(a->dr, nume);
    } else {
        if (!a->st) {
            aux = a->dr;
            free(a->nume);
            distruge_dir(a->directories);
            distruge_fis(a->files);
            free(a);
            return aux;
        } else if (!a->dr) {
            aux = a->st;
            free(a->nume);
            distruge_dir(a->directories);
            distruge_fis(a->files);
            free(a);
            return aux;
        } else {
            free(a->nume);
        }
        aux = valoare_minima_dir(a->dr);
        a->nume = strdup(aux->nume);
        a->dr = Sterge_Dir(a->dr, aux->nume);
    }
    return a;
}

TFis Sterge_Fis(TFis a, char *nume) {
    TFis aux = NULL;
    if (!a) return NULL;;
    if (cmp_sir(nume, a->nume) < 0) {
        a->st = Sterge_Fis(a->st, nume);
    } else if (cmp_sir(nume, a->nume) > 0) {
        a->dr = Sterge_Fis(a->dr, nume);
    } else {
        if (!a->st) {
            aux = a->dr;
            free(a->nume);
            free(a);
            return aux;
        } else if (!a->dr) {
            aux = a->st;
            free(a->nume);
            free(a);
            return aux;
        } else {
            free(a->nume);
        }
        aux = valoare_minima_fis(a->dr);
        a->nume = strdup(aux->nume);
        a->dr = Sterge_Fis(a->dr, aux->nume);
    }
    return a;
}

int main() {
    TDir dir = NULL, dir_curent = NULL, dir_nou = NULL;
    TLG L = NULL, pL = NULL;
    char *linie = NULL, *p = NULL;
    int cont = 0;
    linie = (char*)calloc(LINIE, sizeof(char));
    Inserare_Dir(&dir, "root", cmp_sir);
    dir_curent = dir;
    while (fgets(linie, LINIE, stdin)) {
        cont = 0;
        if(linie[strlen(linie) - 1] == '\n') {
            linie[strlen(linie) - 1] = '\0';
        }
        p = strtok(linie, " ");
        if (strcmp(p, "mkdir") == 0) {
            p = strtok(NULL, " ");
            if (Cautare_Fis(dir_curent, p, &cont, &dir_nou) == 0) {
                if (Cautare_Dir2(dir_curent->directories, p, &cont, &dir_nou) == 0) {
                    Inserare_Dir(&dir_curent, p, cmp_sir);
                } else {
                    printf("Directory %s already exists!\n", p);
                }
            } else {
                printf("File %s already exists!\n", p);
            }
        } else if (strcmp(p, "touch") == 0 ) {
            p = strtok(NULL, " ");
            if (Cautare_Fis(dir_curent, p, &cont, &dir_nou) == 0) {
                if (Cautare_Dir(dir_curent, p, &cont, &dir_nou) == 0) {
                    Inserare_Fis(&dir_curent, p, cmp_sir);
                } else {
                    printf("Directory %s already exists!\n", p);
                }
            } else {
                printf("File %s already exists!\n", p);
            }
        } else if (strcmp(p, "cd") == 0 ) {
            p = strtok(NULL, " ");
            if (strcmp(p, "..") == 0) {
                dir_curent = dir_curent->parent;
            } else {
                if(!Selectare_Director(dir_curent->directories, p, cmp_sir, &dir_nou)) {
                    printf("Directory not found!\n");
                } else {
                    dir_curent = dir_nou;
                }
            }
        } else if (strcmp(p, "ls") == 0 ) {
            Afisare_Dir(dir_curent->directories, &L);
            for (pL = L; pL != NULL; pL = pL->urm) {
                printf("%s ", pL->cuv);
            }
            Distruge_Lista(&L);
            Afisare_Fis(dir_curent->files, &L);
            for (pL = L; pL != NULL; pL = pL->urm) {
                printf("%s ", pL->cuv);
            }
            Distruge_Lista(&L);
            printf("\n");
        } else if (strcmp(p, "rmdir") == 0 ) {
            p = strtok(NULL, " ");
            if (Cautare_Dir2(dir_curent->directories, p, &cont, &dir_nou) == 1) {
                dir_curent->directories = Sterge_Dir(dir_curent->directories, p);
            } else {
                printf("Directory %s doesn't exist!\n", p);
            }
        } else if (strcmp(p, "rm") == 0 ) {
            p = strtok(NULL, " ");
            if (Cautare_Fis(dir_curent, p, &cont, &dir_nou) == 1) {
                dir_curent->files = Sterge_Fis(dir_curent->files, p);
            } else {
                printf("File %s doesn't exist!\n", p);
            }
        } else if (strcmp(p, "pwd") == 0 ) {
            Afisare_Cale(dir_curent);
        } else if (strcmp(p, "find") == 0 ) {
            p = strtok(NULL, " ");
            if (strcmp(p, "-d") == 0) {
                p = strtok(NULL, " ");
                if (Cautare_Dir(dir, p, &cont, &dir_nou)) {
                    printf("Directory %s found!\n", p);
                    Afisare_Cale(dir_nou);
                } else {
                    printf("Directory %s not found!\n", p);
                }
            } else if (strcmp(p, "-f") == 0) {
                p = strtok(NULL, " ");
                if (Cautare_Fis(dir, p, &cont, &dir_nou)) {
                    printf("File %s found!\n", p);
                    Afisare_Cale(dir_nou);
                } else {
                    printf("File %s not found!\n", p);
                }
            }
        } else if (strcmp(p, "quit") == 0) {
            Distruge_Arb(&dir);
            break;
        }
        p = strtok(NULL, " ");
    }
    free(linie);
    return 0;
}