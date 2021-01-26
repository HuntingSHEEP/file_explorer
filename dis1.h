#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp, *fp2;
char path[2035];
char path2[2035];
char name[2035];
char nazwy[512][2035];
char typy[512];
char sciezka[64][2035] = {""};

int lp;
char lewy_panel1[2][5] = {"home", "$USER"};
char lewy_panel2[5][10] = {"home", "$USER", ".local", "share", "Trash"};
char lewy_panel1_1[6][20] = {"Pulpit", "Dokumenty", "Pobrane", "Muzyka", "Obrazy", "Wideo"};
char komenda[2][24] = {"ls -l /", "xdg-open /"};

int glebokosc = 0;
char *wykonac = NULL;
int rozmiarWykonac = 8;

char *wykon2 = NULL;
char wykon_dane[10] = "ls  /";

void initialize(){
    wykonac = malloc(sizeof(char)*rozmiarWykonac);
    strcpy(wykonac, komenda[0]);

    wykon2 = malloc(sizeof(char)*rozmiarWykonac);
    strcpy(wykon2, wykon_dane);
}

void freeMemory(){
    free(wykonac);
    free(wykon2);
}


char etykieta_WSZYSTKIE[2][50] = {"TYLKO GŁÓWNE PLIKI","WYŚWIETL UKRYTE"};
int flagaNIE_WSZYSTKIE = 1;

int flaga_KATALOG_lub_PLIK = 0;

void aktualizuj_dane_lokalizacji(){
  /* zerowanie list nazwy i typy */
  memset(typy, 0, sizeof(typy));
  memset(nazwy, 0, sizeof(nazwy));
  lp = 0;

  /* wykonaj komendy */
  fp = popen(wykonac, "r");
  fp2 = popen(wykon2, "r");

  /* Jeśli coś pójdzie nie tak, to zakończ pracę*/
  if ((fp == NULL) || (fp2 == NULL)) {printf("BŁĄD WYKONANIA KOMENDY\n" ); exit(1);}

  /* Czytaj dane po kolei. */

  fgets(path, sizeof(path), fp);
  while ((fgets(path, sizeof(path), fp) != NULL)) {
    fgets(path2, sizeof(path2), fp2); //kichana optymalizacja
    memset(name, 0, sizeof(name));

    int r = 0;
    for (int z=0; z<strlen(path2); z++){
      int znak = path2[z];

      if (znak == ' '){
        name[r] = '\\';
        r++;
      }
      if (znak != '\n'){
        name[r] = znak;
        r++;
      }
    }
    typy[lp]= path[0];
    strcpy(nazwy[lp], name);
    lp++;

    path[strlen(path)-1]='\0';
    path2[strlen(path2)-1]='\0';
    printf("[%c][%s] [%s][%s]\n",typy[lp-1],nazwy[lp-1], path, path2);
  }
  printf("\n\n");

  /*W tym momencie zmienne przechowują:
      lp: ilość pozycji
      nazwy: nazwy pozycji (plików, katalogów)
      typy : oraz ich nazwy
  */

  /* close */
  pclose(fp);
  pclose(fp2);
}
