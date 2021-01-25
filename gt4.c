#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "dis1.h"
//komenda do kompilacji kodu: gcc gt4.c -o frontend `pkg-config --cflags --libs gtk+-3.0` && ./frontend

void wymaluj_mie_guziki();
void wymaluj_mie_frontend();
void myCSS();

int imax(int a, int b){
  if (a>b)
    return a;
  else
    return b;
}

GtkWidget *guziki[520];
GtkWidget *powrot, *pokaz_wszystkie;
GtkWidget *window, *ggrid, *grid, *label, *label1, *button1, *button2;

GtkWidget *guziki_lewa_komora[8];
GtkWidget *grid1_0, *grid1_1, *grid1_2, *grid2_0, *grid2_1, *grid2_2;
GtkWidget *w_scrolled;
GtkWidget *separator, *separator1, *separator2;

char nazwy_lewa_komora[8][56] = {"Katalog domowy", "Pulpit", "Dokumenty","Pobrane", "Muzyka", "Obrazy",  "Wideo", "Kosz"};
char typy_przyciskow[8][12]= {"plik", "folder"};


void aktualizuj_komendy_oraz_wykonaj(){
  memset(wykonac, 0, sizeof(wykonac));
  strcpy(wykonac, komenda[flaga_KATALOG_lub_PLIK]);

  memset(wykon2, 0, sizeof(wykon2));
  strcpy(wykon2, wykon_dane);

  //aktualizuję komendy
  for(int i=0; i<glebokosc; i++){
      //TODO: zrobić realokację dla wykonac oraz wykon2
    strcat(wykonac, sciezka[i]);
    strcat(wykonac, "/");

    strcat(wykon2, sciezka[i]);
    strcat(wykon2, "/");
  }

  // usuwam siatkę wraz z zawartością z okna
  gtk_container_remove(GTK_CONTAINER(w_scrolled), ggrid);

  //skoro usuwam guziki z okna, to wypadałoby usunąć je też z RAMu
  memset(guziki, 0, sizeof(GtkWidget));

  //tworzę i dodaję świeżą siatkę do okna
  ggrid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(w_scrolled), ggrid);

  //rysuję aktualne przyciski
  wymaluj_mie_guziki();
}


void zrob_cos(GtkWidget *przycisk, gpointer data){
  //odnotowuję numer przycisku, który się głosił
  int number = (int)data;
  g_print("%d A JA WIEM\n",number);

  if (typy[number]=='-'){
    flaga_KATALOG_lub_PLIK = 1;

    strcpy(sciezka[glebokosc], nazwy[number]);
    glebokosc++;
    memset(wykonac, 0, sizeof(wykonac));
    strcpy(wykonac, komenda[flaga_KATALOG_lub_PLIK]);

    for(int i=0; i<glebokosc; i++){
        //TODO: zrobić realokację dla wykonac
      strcat(wykonac, sciezka[i]);
      strcat(wykonac, "/");
    }
    glebokosc--;
    memset(sciezka[glebokosc], 0, sizeof(sciezka[0]));
    flaga_KATALOG_lub_PLIK = 0;
    fp = popen(wykonac, "r");

  }else if((-18<=number) && (number<=-11)){
    number = (-number)-11;
    glebokosc = imax(glebokosc, 3);

    if (number == 0){
      for(int i=0; i<glebokosc; i++){
        memset(sciezka[i], 0, sizeof(sciezka[0]));
        if (i<2)
          strcpy(sciezka[i], lewy_panel1[i]);
        }
      glebokosc = 2;
    }

    if ((0<number) && (number<7)){
      for(int i=0; i<glebokosc; i++){
        memset(sciezka[i], 0, sizeof(sciezka[0]));
        if (i<2)
          strcpy(sciezka[i], lewy_panel1[i]);
        if (i==2)
          strcpy(sciezka[i], lewy_panel1_1[number-1]);
        }
      glebokosc = 3;
    }

    if (number==7){
      glebokosc = imax(glebokosc, 5);

      for(int i=0; i<glebokosc; i++){
        memset(sciezka[i], 0, sizeof(sciezka[0]));
        if (i<5)
          strcpy(sciezka[i], lewy_panel2[i]);
        }
      glebokosc = 5;
    }

    aktualizuj_komendy_oraz_wykonaj();

    }else if (number==-1){
    //podpiąć flagę do malowania i podmienić komendy i zaktualizować
    if (flagaNIE_WSZYSTKIE==0){
      //wykon_dane = "ls  /";
      memset(wykon_dane, 0, sizeof(wykon_dane));
      strcpy(wykon_dane, "ls  /");

      //komenda = {"ls -l /", "xdg-open /"};
      memset(komenda[0], 0, sizeof(komenda[0]));
      strcpy(komenda[0], "ls -l /");

      flagaNIE_WSZYSTKIE=1;
    }else{
      //wykon_dane = "ls -a /";
      memset(wykon_dane, 0, sizeof(wykon_dane));
      strcpy(wykon_dane, "ls -a /");

      //komenda = {"ls -la /", "xdg-open /"};
      memset(komenda[0], 0, sizeof(komenda[0]));
      strcpy(komenda[0], "ls -la /");

      flagaNIE_WSZYSTKIE=0;
    }
    aktualizuj_komendy_oraz_wykonaj();

  }else if(number==-2){
    if (glebokosc > 0){
      glebokosc--;
      memset(sciezka[glebokosc], 0, sizeof(sciezka[0]));
    }
    flaga_KATALOG_lub_PLIK = 0;
    aktualizuj_komendy_oraz_wykonaj();

  }else if((typy[number]=='d') || (typy[number]=='l') ){
    flaga_KATALOG_lub_PLIK = 0;
    strcpy(sciezka[glebokosc], nazwy[number]);
    glebokosc++;
    aktualizuj_komendy_oraz_wykonaj();
  }
  g_print(" %s\n", wykonac);
}





int main(int argc, char **argv){
  //ta linijka jest potrzebna, by gtk ruszyło
  gtk_init(&argc, &argv);
  myCSS();

  //tworzenie nowego okna; obsługa sygnału "zamknij" czerwonego krzyżyka; domyślny rozmiar okna;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_set_size_request(window, 580, 380);


  //rozmieszczenie elementów GUI
  wymaluj_mie_frontend();

  //rozmieszczenie przycisków na siatce
  wymaluj_mie_guziki();

  //uruchomienie głównej pętli GTK obsługującej sygnały
  gtk_main();

  return 0;
}

void myCSS(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "mystyle.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}

void wymaluj_mie_frontend(){
  grid = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (window), grid);

  // GRID1_0
  grid1_0 = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), grid1_0, 0, 0, 1, 1);

  for (int u=0; u<8; u++){
    char nazwa[80];
    strcpy(nazwa, nazwy_lewa_komora[u]);
    for (int t=0; t<strlen(nazwy_lewa_komora[0])-strlen(nazwy_lewa_komora[u]); t++)
      strcat(nazwa, " ");

    guziki_lewa_komora[u] = gtk_button_new_with_label (nazwa);
    gtk_grid_attach (GTK_GRID (grid1_0), guziki_lewa_komora[u], 0, u, 1, 1);
    g_signal_connect(guziki_lewa_komora[u], "clicked", G_CALLBACK(zrob_cos), (gpointer) (-u)-11);
    gtk_widget_set_halign(guziki_lewa_komora[u], GTK_ALIGN_START);
    gtk_widget_set_name(guziki_lewa_komora[u], "plik");
    //gtk_label_set_xalign(label1, 0.0);
    if (u==0)
      gtk_widget_set_margin_top(guziki_lewa_komora[u], 10);
    gtk_widget_set_margin_bottom(guziki_lewa_komora[u], 10);
    gtk_widget_set_margin_start(guziki_lewa_komora[u], 5);
    gtk_widget_set_margin_end(guziki_lewa_komora[u], 2);
  }

  separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_grid_attach (GTK_GRID (grid1_0), separator1, 0, 8, 1, 1);
  gtk_widget_set_margin_top(separator1, 20);
  gtk_widget_set_margin_start(separator1, 2);


  // GRID1_1
  grid1_1 = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), grid1_1, 1,0,1,1);

  separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_grid_attach (GTK_GRID (grid1_1), separator, 0, 0, 1, 1);
  gtk_widget_set_vexpand(separator, TRUE);
  gtk_widget_set_margin_bottom(separator, 3);
  gtk_widget_set_margin_top(separator, 3);
  gtk_widget_set_margin_start(separator, 5);
  gtk_widget_set_margin_end(separator, 3);

  // GRID1_2
  grid1_2 = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), grid1_2, 2,0,1,1);

  // GRID2_0
  grid2_0 = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid1_2), grid2_0, 0,0,1,1);

  powrot = gtk_button_new_with_label ("<");
  gtk_grid_attach (GTK_GRID (grid2_0), powrot, 0, 0, 1, 1);
  g_signal_connect(powrot, "clicked", G_CALLBACK(zrob_cos), (gpointer) -2);
  gtk_widget_set_halign(powrot, GTK_ALIGN_FILL);
  gtk_widget_set_name(powrot, "plik");
  //gtk_label_set_xalign(label1, 0.0);
  gtk_widget_set_margin_top(powrot, 10);
  gtk_widget_set_margin_bottom(powrot, 10);
  gtk_widget_set_margin_start(powrot, 5);
  gtk_widget_set_margin_end(powrot, 2);


  label1 = gtk_label_new ("XD");
  gtk_grid_attach (GTK_GRID (grid2_0), label1, 1, 0, 1, 1);
  gtk_widget_set_halign(label1, GTK_ALIGN_FILL);
  //gtk_widget_set_name(label1, "plik");
  gtk_widget_set_hexpand(label1, TRUE);
  //gtk_label_set_xalign(label1, 0.0);
  gtk_widget_set_margin_top(label1, 10);
  gtk_widget_set_margin_bottom(label1, 10);
  gtk_widget_set_margin_start(label1, 5);
  gtk_widget_set_margin_end(label1, 2);



  pokaz_wszystkie = gtk_button_new_with_label ("[*]");
  gtk_grid_attach (GTK_GRID (grid2_0), pokaz_wszystkie, 2, 0, 1, 1);
  g_signal_connect(pokaz_wszystkie, "clicked", G_CALLBACK(zrob_cos), (gpointer) -1);
  gtk_widget_set_halign(pokaz_wszystkie, GTK_ALIGN_FILL);
  gtk_widget_set_name(pokaz_wszystkie, "plik");
  //gtk_label_set_xalign(label1, 0.0);
  gtk_widget_set_margin_top(pokaz_wszystkie, 10);
  gtk_widget_set_margin_bottom(pokaz_wszystkie, 10);
  gtk_widget_set_margin_start(pokaz_wszystkie, 5);
  gtk_widget_set_margin_end(pokaz_wszystkie, 9);

  // GRID2_1
  grid2_1 = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid1_2), grid2_1, 0,1,1,1);

  separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_grid_attach (GTK_GRID (grid2_1), separator2, 0, 0, 1, 1);
  gtk_widget_set_hexpand(separator2, TRUE);
  //gtk_widget_set_margin_top(separator2, 1);
  gtk_widget_set_margin_bottom(separator2, 10);
  gtk_widget_set_margin_start(separator2, 2);
  gtk_widget_set_margin_end(separator2, 2);

  // GRID2_2
  grid2_2 = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid1_2), grid2_2, 0,2,1,1);

  w_scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_grid_attach (GTK_GRID (grid2_2), w_scrolled, 0, 0, 1, 1);
  gtk_widget_set_hexpand(w_scrolled, TRUE);
  gtk_widget_set_vexpand(w_scrolled, TRUE);

  ggrid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(w_scrolled), ggrid);
}

void wymaluj_mie_guziki(){
  aktualizuj_dane_lokalizacji();

  int index_typu = 0;
  int index_przycisku = 0;

  //dynamiczne tworzenie i rozmieszczanie przycisków na siatce z poziomu pętli
  for (int warunek=0; warunek<2; warunek++){
    for (int i=2-(2)*flagaNIE_WSZYSTKIE; i<lp; i++){

      if ( (warunek==0) && (!( (typy[i]=='d') || (typy[i]=='l') )) ){
        continue;
      }else if( (warunek==1) && (( (typy[i]=='d') || (typy[i]=='l') )) ){
        continue;
      }

      if ((typy[i]=='d')||(typy[i]=='l'))
        index_typu=1;
      else
        index_typu=0;

      char dest[256] = "";
      sprintf(dest, "%s", nazwy[i]);

      //char numer[10];
      //sprintf(numer, " [%c]", typy[i]);
      //strcat(dest, numer);

      guziki[index_przycisku] = gtk_button_new_with_label(dest);
      g_signal_connect(guziki[index_przycisku], "clicked", G_CALLBACK(zrob_cos), (gpointer) i);
      gtk_widget_set_name(guziki[index_przycisku], typy_przyciskow[index_typu]);
      index_przycisku++;
    }
  }

  index_przycisku = 0;
  for (int i=2-(2)*flagaNIE_WSZYSTKIE; i<lp; i++){
    gtk_grid_attach(GTK_GRID(ggrid), guziki[index_przycisku], (i-2+(2)*flagaNIE_WSZYSTKIE)/20, (i-2+(2)*flagaNIE_WSZYSTKIE)%20+1 , 1, 1);
    index_przycisku++;
  }


  gtk_widget_show_all(window);
}
