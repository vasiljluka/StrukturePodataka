#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godina;
    struct Osoba *sljedeca;
} Osoba;

Osoba *novaOsoba(char ime[], char prezime[], int godina) {
    Osoba *nova = malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        exit(1);
    }

    strcpy(nova->ime, ime);
    strcpy(nova->prezime, prezime);
    nova->godina = godina;
    nova->sljedeca = NULL;

    return nova;
}

void dodajNaPocetak(Osoba **glava, char ime[], char prezime[], int godina) {
    Osoba *nova = novaOsoba(ime, prezime, godina);
    nova->sljedeca = *glava;
    *glava = nova;
}

void dodajNaKraj(Osoba **glava, char ime[], char prezime[], int godina) {
    Osoba *nova = novaOsoba(ime, prezime, godina);

    if (*glava == NULL) {
        *glava = nova;
        return;
    }

    Osoba *temp = *glava;

    while (temp->sljedeca != NULL) {
        temp = temp->sljedeca;
    }

    temp->sljedeca = nova;
}

void ispisiListu(Osoba *glava) {
    Osoba *temp = glava;

    while (temp != NULL) {
        printf("%s %s, godina rodjenja: %d\n",
               temp->ime, temp->prezime, temp->godina);
        temp = temp->sljedeca;
    }
}

Osoba *pronadjiOsobu(Osoba *glava, char prezime[]) {
    Osoba *temp = glava;

    while (temp != NULL) {
        if (strcmp(temp->prezime, prezime) == 0) {
            return temp;
        }

        temp = temp->sljedeca;
    }

    return NULL;
}

void obrisiOsobu(Osoba **glava, char prezime[]) {
    Osoba *temp = *glava;
    Osoba *prethodni = NULL;

    while (temp != NULL && strcmp(temp->prezime, prezime) != 0) {
        prethodni = temp;
        temp = temp->sljedeca;
    }

    if (temp == NULL) {
        printf("Osoba nije pronadjena.\n");
        return;
    }

    if (prethodni == NULL) {
        *glava = temp->sljedeca;
    } else {
        prethodni->sljedeca = temp->sljedeca;
    }

    free(temp);
    printf("Osoba je obrisana.\n");
}

void dodajIza(Osoba *glava, char trazeno[], char ime[],
              char prezime[], int godina) {
    Osoba *temp = pronadjiOsobu(glava, trazeno);

    if (temp == NULL) {
        printf("Osoba nije pronadjena.\n");
        return;
    }

    Osoba *nova = novaOsoba(ime, prezime, godina);

    nova->sljedeca = temp->sljedeca;
    temp->sljedeca = nova;
}

void dodajIspred(Osoba **glava, char trazeno[], char ime[],
                 char prezime[], int godina) {
    Osoba *temp = *glava;
    Osoba *prethodni = NULL;

    while (temp != NULL && strcmp(temp->prezime, trazeno) != 0) {
        prethodni = temp;
        temp = temp->sljedeca;
    }

    if (temp == NULL) {
        printf("Osoba nije pronadjena.\n");
        return;
    }

    Osoba *nova = novaOsoba(ime, prezime, godina);
    nova->sljedeca = temp;

    if (prethodni == NULL) {
        *glava = nova;
    } else {
        prethodni->sljedeca = nova;
    }
}

void sortirajListu(Osoba *glava) {
    Osoba *i, *j;
    char ime[50], prezime[50];
    int godina;

    for (i = glava; i != NULL; i = i->sljedeca) {
        for (j = i->sljedeca; j != NULL; j = j->sljedeca) {
            if (strcmp(i->prezime, j->prezime) > 0) {
                strcpy(ime, i->ime);
                strcpy(prezime, i->prezime);
                godina = i->godina;

                strcpy(i->ime, j->ime);
                strcpy(i->prezime, j->prezime);
                i->godina = j->godina;

                strcpy(j->ime, ime);
                strcpy(j->prezime, prezime);
                j->godina = godina;
            }
        }
    }
}

void upisiUDatoteku(Osoba *glava) {
    FILE *dat = fopen("osobe.txt", "w");

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return;
    }

    Osoba *temp = glava;

    while (temp != NULL) {
        fprintf(dat, "%s %s %d\n",
                temp->ime, temp->prezime, temp->godina);
        temp = temp->sljedeca;
    }

    fclose(dat);
    printf("Lista je spremljena u datoteku.\n");
}

void procitajDatoteku(Osoba **glava) {
    FILE *dat = fopen("osobe.txt", "r");

    if (dat == NULL) {
        printf("Datoteka ne postoji ili se ne moze otvoriti.\n");
        return;
    }

    Osoba *temp = *glava;

    while (temp != NULL) {
        Osoba *zaBrisanje = temp;
        temp = temp->sljedeca;
        free(zaBrisanje);
    }

    *glava = NULL;

    char ime[50], prezime[50];
    int godina;

    while (fscanf(dat, "%49s %49s %d", ime, prezime, &godina) == 3) {
        dodajNaKraj(glava, ime, prezime, godina);
    }

    fclose(dat);
    printf("Lista je ucitana iz datoteke.\n");
}

int main() {
    Osoba *glava = NULL;
    int izbor, godina;
    char ime[50], prezime[50], trazeno[50];

    do {
        printf("\n1. Dodaj na pocetak");
        printf("\n2. Dodaj na kraj");
        printf("\n3. Ispisi listu");
        printf("\n4. Pronadji osobu");
        printf("\n5. Obrisi osobu");
        printf("\n6. Dodaj iza osobe");
        printf("\n7. Dodaj ispred osobe");
        printf("\n8. Sortiraj po prezimenu");
        printf("\n9. Upisi listu u datoteku");
        printf("\n10. Procitaj listu iz datoteke");
        printf("\n0. Izlaz");
        printf("\nOdabir: ");

        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
            case 2:
                printf("Ime: ");
                scanf("%49s", ime);

                printf("Prezime: ");
                scanf("%49s", prezime);

                printf("Godina rodjenja: ");
                scanf("%d", &godina);

                if (izbor == 1) {
                    dodajNaPocetak(&glava, ime, prezime, godina);
                } else {
                    dodajNaKraj(&glava, ime, prezime, godina);
                }
                break;

            case 3:
                ispisiListu(glava);
                break;

            case 4:
                printf("Prezime osobe: ");
                scanf("%49s", trazeno);

                if (pronadjiOsobu(glava, trazeno) != NULL) {
                    printf("Osoba je pronadjena.\n");
                } else {
                    printf("Osoba nije pronadjena.\n");
                }
                break;

            case 5:
                printf("Prezime osobe za brisanje: ");
                scanf("%49s", trazeno);
                obrisiOsobu(&glava, trazeno);
                break;

            case 6:
            case 7:
                printf("Prezime osobe ispred/iza koje dodajemo: ");
                scanf("%49s", trazeno);

                printf("Ime nove osobe: ");
                scanf("%49s", ime);

                printf("Prezime nove osobe: ");
                scanf("%49s", prezime);

                printf("Godina rodjenja: ");
                scanf("%d", &godina);

                if (izbor == 6) {
                    dodajIza(glava, trazeno, ime, prezime, godina);
                } else {
                    dodajIspred(&glava, trazeno, ime, prezime, godina);
                }
                break;

            case 8:
                sortirajListu(glava);
                printf("Lista je sortirana.\n");
                break;

            case 9:
                upisiUDatoteku(glava);
                break;

            case 10:
                procitajDatoteku(&glava);
                break;

            case 0:
                printf("Kraj programa.\n");
                break;

            default:
                printf("Neispravan odabir.\n");
        }

    } while (izbor != 0);

    while (glava != NULL) {
        Osoba *temp = glava;
        glava = glava->sljedeca;
        free(temp);
    }

    return 0;
}
