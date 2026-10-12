#include <stdio.h>
#include <stdlib.h>

#define MAX_BODOVA 50.0

typedef struct {
    char ime[32];
    char prezime[32];
    double bodovi;
} Student;

int main(void)
{
    FILE *fp;
    Student *studenti;
    int broj_redaka = 0, i;
    int c, zadnji = '\n';

    fp = fopen("studenti.txt", "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n')
            broj_redaka++;
        zadnji = c;
    }
   
    if (zadnji != '\n')
        broj_redaka++;

    if (broj_redaka == 0) {
        printf("Datoteka je prazna.\n");
        fclose(fp);
        return 0;
    }

    
    studenti = (Student *)malloc(broj_redaka * sizeof(Student));
    if (studenti == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        fclose(fp);
        return 1;
    }

   
    rewind(fp);
    for (i = 0; i < broj_redaka; i++) {
        if (fscanf(fp, "%31s %31s %lf",
                   studenti[i].ime,
                   studenti[i].prezime,
                   &studenti[i].bodovi) != 3) {
            printf("Greska pri citanju retka %d!\n", i + 1);
            broj_redaka = i;
            break;
        }
    }
    fclose(fp);

    
    printf("%-15s %-15s %10s %10s\n", "Ime", "Prezime", "Apsolutno", "Relativno");
    printf("-------------------------------------------------------\n");
    for (i = 0; i < broj_redaka; i++) {
        double relativni = studenti[i].bodovi / MAX_BODOVA * 100;
        printf("%-15s %-15s %10.2f %9.2f%%\n",
               studenti[i].ime,
               studenti[i].prezime,
               studenti[i].bodovi,
               relativni);
    }

    free(studenti);
    return 0;
}
