#include <stdio.h>
#include <stdlib.h>

typedef struct Clan {
    int koef;
    int exp;
    struct Clan *sljedeci;
} Clan;

Clan *dodajClan(Clan *glava, int koef, int exp) {
    if (koef == 0)
        return glava;

    Clan *temp = glava;
    Clan *prethodni = NULL;

    while (temp != NULL && temp->exp > exp) {
        prethodni = temp;
        temp = temp->sljedeci;
    }

    if (temp != NULL && temp->exp == exp) {
        temp->koef += koef;
        return glava;
    }

    Clan *novi = malloc(sizeof(Clan));

    if (novi == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return glava;
    }

    novi->koef = koef;
    novi->exp = exp;
    novi->sljedeci = temp;

    if (prethodni == NULL)
        glava = novi;
    else
        prethodni->sljedeci = novi;

    return glava;
}

Clan *ucitajPolinom(FILE *datoteka) {
    int n, koef, exp;
    Clan *glava = NULL;

    if (fscanf(datoteka, "%d", &n) != 1)
        return NULL;

    for (int i = 0; i < n; i++) {
        if (fscanf(datoteka, "%d %d", &koef, &exp) != 2)
            break;

        glava = dodajClan(glava, koef, exp);
    }

    return glava;
}

Clan *zbrojiPolinome(Clan *p, Clan *q) {
    Clan *rezultat = NULL;

    while (p != NULL) {
        rezultat = dodajClan(rezultat, p->koef, p->exp);
        p = p->sljedeci;
    }

    while (q != NULL) {
        rezultat = dodajClan(rezultat, q->koef, q->exp);
        q = q->sljedeci;
    }

    return rezultat;
}

Clan *pomnoziPolinome(Clan *p, Clan *q) {
    Clan *rezultat = NULL;
    Clan *a = p;

    while (a != NULL) {
        Clan *b = q;

        while (b != NULL) {
            int koef = a->koef * b->koef;
            int exp = a->exp + b->exp;

            rezultat = dodajClan(rezultat, koef, exp);
            b = b->sljedeci;
        }

        a = a->sljedeci;
    }

    return rezultat;
}

void ispisiPolinom(Clan *glava) {
    if (glava == NULL) {
        printf("0\n");
        return;
    }

    Clan *temp = glava;
    int prvi = 1;

    while (temp != NULL) {
        if (temp->koef != 0) {
            if (!prvi && temp->koef > 0)
                printf(" + ");

            if (temp->koef < 0)
                printf(" - ");

            int vrijednost = abs(temp->koef);

            if (temp->exp == 0)
                printf("%d", vrijednost);
            else if (temp->exp == 1)
                printf("%dx", vrijednost);
            else
                printf("%dx^%d", vrijednost, temp->exp);

            prvi = 0;
        }

        temp = temp->sljedeci;
    }

    if (prvi)
        printf("0");

    printf("\n");
}

void oslobodiListu(Clan *glava) {
    while (glava != NULL) {
        Clan *temp = glava;
        glava = glava->sljedeci;
        free(temp);
    }
}

int main() {
    FILE *datoteka = fopen("polinomi.txt", "r");

    if (datoteka == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    Clan *p = ucitajPolinom(datoteka);
    Clan *q = ucitajPolinom(datoteka);

    fclose(datoteka);

    Clan *zbroj = zbrojiPolinome(p, q);
    Clan *umnozak = pomnoziPolinome(p, q);

    printf("Prvi polinom: ");
    ispisiPolinom(p);

    printf("Drugi polinom: ");
    ispisiPolinom(q);

    printf("Zbroj: ");
    ispisiPolinom(zbroj);

    printf("Umnozak: ");
    ispisiPolinom(umnozak);

    oslobodiListu(p);
    oslobodiListu(q);
    oslobodiListu(zbroj);
    oslobodiListu(umnozak);

    return 0;
}
