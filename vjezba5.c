#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cvor {
    double vrijednost;
    struct Cvor *sljedeci;
} Cvor;

int push(Cvor **vrh, double vrijednost) {
    Cvor *novi = malloc(sizeof(Cvor));

    if (novi == NULL)
        return 0;

    novi->vrijednost = vrijednost;
    novi->sljedeci = *vrh;
    *vrh = novi;

    return 1;
}

int pop(Cvor **vrh, double *vrijednost) {
    if (*vrh == NULL)
        return 0;

    Cvor *temp = *vrh;
    *vrijednost = temp->vrijednost;
    *vrh = temp->sljedeci;
    free(temp);

    return 1;
}

void oslobodiStog(Cvor **vrh) {
    double vrijednost;

    while (pop(vrh, &vrijednost));
}

int izracunaj(FILE *datoteka, double *rezultat) {
    Cvor *vrh = NULL;
    char token[100];
    double a, b, broj;
    char *kraj;

    while (fscanf(datoteka, "%99s", token) == 1) {
        broj = strtod(token, &kraj);

        if (*kraj == '\0' && kraj != token) {
            if (!push(&vrh, broj)) {
                oslobodiStog(&vrh);
                return 0;
            }
        }
        else {
            if (!pop(&vrh, &b) || !pop(&vrh, &a)) {
                oslobodiStog(&vrh);
                return 0;
            }

            switch (token[0]) {
                case '+':
                    if (token[1] != '\0') {
                        push(&vrh, a);
                        push(&vrh, b);
                        oslobodiStog(&vrh);
                        return 0;
                    }
                    broj = a + b;
                    break;

                case '-':
                    if (token[1] != '\0') {
                        push(&vrh, a);
                        push(&vrh, b);
                        oslobodiStog(&vrh);
                        return 0;
                    }
                    broj = a - b;
                    break;

                case '*':
                    if (token[1] != '\0') {
                        push(&vrh, a);
                        push(&vrh, b);
                        oslobodiStog(&vrh);
                        return 0;
                    }
                    broj = a * b;
                    break;

                case '/':
                    if (token[1] != '\0' || b == 0) {
                        push(&vrh, a);
                        push(&vrh, b);
                        oslobodiStog(&vrh);
                        return 0;
                    }
                    broj = a / b;
                    break;

                default:
                    push(&vrh, a);
                    push(&vrh, b);
                    oslobodiStog(&vrh);
                    return 0;
            }

            if (!push(&vrh, broj)) {
                oslobodiStog(&vrh);
                return 0;
            }
        }
    }

    if (!pop(&vrh, rezultat) || vrh != NULL) {
        oslobodiStog(&vrh);
        return 0;
    }

    return 1;
}

int main() {
    FILE *datoteka = fopen("izraz.txt", "r");

    if (datoteka == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    double rezultat;

    if (izracunaj(datoteka, &rezultat)) {
        printf("Rezultat izraza je: %.2f\n", rezultat);
    }
    else {
        printf("Neispravan izraz.\n");
    }

    fclose(datoteka);

    return 0;
}
