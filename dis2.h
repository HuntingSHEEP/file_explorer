#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char **sciezka;

int maxAmountOfFiles = 64;
char *typy;
char **nazwy = NULL;

FILE *fp, *fp2;

char oneCharacter;
int lp;
char lewy_panel1[2][5] = {"home", "$USER"};
char lewy_panel2[5][10] = {"home", "$USER", ".local", "share", "Trash"};
char lewy_panel1_1[6][20] = {"Pulpit", "Dokumenty", "Pobrane", "Muzyka", "Obrazy", "Wideo"};
char komenda[2][24] = {"ls -l /", "xdg-open /"};

int maxDepth = 1;
int glebokosc = 0;
char *wykonac = NULL;
int rozmiarWykonac = 8;

char *wykon2 = NULL;
char wykon_dane[10] = "ls  /";

int flagaNIE_WSZYSTKIE = 1;
int flaga_KATALOG_lub_PLIK = 0;

void initialize(){
    wykonac = malloc(sizeof(char)*rozmiarWykonac);
    strcpy(wykonac, komenda[0]);

    wykon2 = malloc(sizeof(char)*rozmiarWykonac);
    strcpy(wykon2, wykon_dane);

    //oneCharacter = malloc(sizeof(char));

    typy = malloc(sizeof(char)*maxAmountOfFiles);
    nazwy = (char **) malloc(sizeof(char *)*maxAmountOfFiles);
    sciezka = (char **) malloc(sizeof(char *) * maxDepth);
}

void freeMemory(){
    free(wykonac);
    free(wykon2);
    //free(oneCharacter);
    free(typy);
    free(nazwy);
    free(sciezka);
}


void aktualizuj_dane_lokalizacji(){
    /* wykonaj komendy */
    fp = popen(wykonac, "r");
    fp2 = popen(wykon2, "r");

    /* Jeśli coś pójdzie nie tak, to zakończ pracę*/
    if ((fp == NULL) || (fp2 == NULL)) {printf("BŁĄD WYKONANIA KOMENDY\n" ); exit(1);}

    /* zerowanie list nazwy i typy */ //TODO:zerowanie list
    strcpy(typy, "");

    /* Czytaj dane po kolei. */
    int typeFlag = 0;
    lp = 0;

    oneCharacter = (char) getc(fp);
    while (!feof(fp)){
        if (typeFlag == 1) {
            if (maxAmountOfFiles < lp + 7) {
                printf("REALLOC [%d]\n\n",maxAmountOfFiles);
                maxAmountOfFiles += 64;
                typy = (char *) realloc(typy, sizeof(char)*maxAmountOfFiles);
                nazwy = (char **) realloc(nazwy, sizeof(char *)*maxAmountOfFiles);
            }
            (*(typy + lp)) = oneCharacter;
            typeFlag = 0;
            lp++;
        }

        if(oneCharacter == '\n'){
            typeFlag = 1;
        }
        oneCharacter = (char) getc(fp);
    }

    lp = 0;
    int maxSizeOfWord = 32;
    int sizeOfWord = 0;
    char *word = malloc(sizeof(char)*maxSizeOfWord);
    oneCharacter = (char) fgetc(fp2);

    while (!feof(fp2)){

        if (oneCharacter == '\n'){
            (*(word + sizeOfWord)) = '\0';
            //printf("SŁOWO: [%s]\n", word);
            *(nazwy + lp) = word;
            lp++;
            word = malloc(sizeof(char)*maxSizeOfWord);
            sizeOfWord = 0;
        }else{
            if(sizeOfWord + 3 > maxSizeOfWord){
                maxSizeOfWord += 16;
                char *tmp = (char *) realloc(word, sizeof(char)*maxSizeOfWord);
                word = tmp;
            }
            if (oneCharacter == ' '){
                (*(word + sizeOfWord)) = '\\';
                sizeOfWord++;
            }

            (*(word + sizeOfWord)) = oneCharacter;
            sizeOfWord++;
        }
        oneCharacter = (char) fgetc(fp2);
    }


    for (int i=0; i<lp; i++){
        g_print("[%c][%s]\n", typy[i], nazwy[i]);
    }
    printf("\n\n--------------------------------------------------------------------\n");


    /*W tym momencie zmienne przechowują:
        lp: ilość pozycji
        nazwy: nazwy pozycji (plików, katalogów)
        typy : oraz ich typy
    */

    /* close */
    pclose(fp);
    pclose(fp2);
}
