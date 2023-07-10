/* Sursa:
          cod pentru implementare comunicare cu server: https://profs.info.uaic.ro/~gcalancea/lab7cliTcpConc.c
          widget-uri: https://zetcode.com/gui/gtk2/ si http://www.manpagez.com/html/gtk3/gtk3-3.16.5/index.php */
#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

  /* portul de conectare la server*/
   int port;

int sd, sd2 , sd3;
char msg[100]=" ";
char msg2[100]=" ";
char msg3[100]=" ";

//stabilim adresa IP
#define IP "127.0.0.1"


  GtkWidget *grid;
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *hbox;

  void show_error(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Nu ati pornit dispozitivul");
  gtk_window_set_title(GTK_WINDOW(dialog), "EROARE");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

 static void toggled_func(GtkWidget* widget, gpointer data)
 {
  if(strcmp((char*)data,"1")==0)
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
        { g_print(" Televizorul este oprit\n",(char*)data);
         strcat(msg," Televizorul este oprit\n");
        }

  if(strcmp((char*)data,"2")==0)
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
        {
          g_print(" Televizorul este pornit\n",(char*)data);
          strcat(msg," Televizorul este pornit\n");
        }
    
  }

static void toggled_func_l(GtkWidget* widget, gpointer data)
{
  if(strcmp((char*)data,"1")==0)
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
         {
           g_print(" Luminile sunt oprite\n",(char*)data);
           strcat(msg2," Luminile sunt oprite\n");
         }


  if(strcmp((char*)data,"2")==0)
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
        { g_print(" Luminile sunt pornite\n",(char*)data);
         strcat(msg2," Luminile sunt oprite\n");
        }
 
    
}

static void toggled_func_a(GtkWidget* widget, gpointer data)
{
  if(strcmp((char*)data,"1")==0)
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
        { g_print(" Aspiratorul este oprit \n",(char*)data);
        strcat(msg3," Aspiratorul este oprit\n");
        }


  if(strcmp((char*)data,"2")==0)
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
        { g_print(" Aspiratorul este pornit \n",(char*)data);
          strcat(msg3," Aspiratorul este pornit\n");
        }

    
}


void value_changed(GtkWidget *hscale, gpointer data) {

   gdouble val = gtk_range_get_value(GTK_RANGE(hscale));
   gchar *str = g_strdup_printf("%.f\n", val);    
   //gtk_label_set_text(GTK_LABEL(win), str);
    g_print("Volumul este: %.f\n",gtk_range_get_value(GTK_RANGE(hscale)));
    strcat(msg," Volumul este:");
    strcat(msg,str);
   
  // g_free(str);
}

void value_changedl(GtkWidget *hscale, gpointer  data) {

    gdouble val = gtk_range_get_value(GTK_RANGE(hscale));
    gchar *str = g_strdup_printf("%.f\n", val);    
    g_print("Intensitatea este %.f\n",gtk_range_get_value(GTK_RANGE(hscale)));
    strcat(msg2," Intensitatea este");
    strcat(msg2,str);
  
}

static void button_entryc(GtkWidget* widget, gpointer data)
{
  gchar *val= gtk_entry_get_text(GTK_ENTRY(data));
  gchar *str = g_strdup_printf("%s\n", val);
  g_print("Canalul: %s\n",gtk_entry_get_text(GTK_ENTRY(data)));
  strcat(msg," Canalul:");
  strcat(msg,str);
}

static void button_entryL(GtkWidget* widget, gpointer data)
{
  gchar *val= gtk_entry_get_text(GTK_ENTRY(data));
  gchar *str = g_strdup_printf("%s\n", val);
  g_print("Lungimea este: %s\n",gtk_entry_get_text(GTK_ENTRY(data)));
  strcat(msg3," Lungimea este:");
  strcat(msg3,str);
}

static void button_entryl(GtkWidget* widget, gpointer data)
{
  gchar *val= gtk_entry_get_text(GTK_ENTRY(data));
  gchar *str = g_strdup_printf("%s\n", val);
  g_print("Latimea este: %s\n",gtk_entry_get_text(GTK_ENTRY(data)));
  strcat(msg3," Latimea este: %s\n");
  strcat(msg3,str);
}

void combo_selected(GtkWidget *combo, gpointer window) 
{
  gchar *val= gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
  gchar *str = g_strdup_printf("%s\n", val);
  g_print("Culoare setata: %s\n",gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)));
  strcat(msg2," Culoarea setata: ");
  strcat(msg2,str);
}


void Meniu()
{
 

  GtkWidget *televizor;
  GtkWidget *lumini;
  GtkWidget *aspirator;
  GtkWidget *quit;

      /* citirea mesajului */
  // while(!quit)
  //bzero (msg3, 100);
  fflush (stdout);
  read (0, msg3, 100);
  
  /* trimiterea mesajului la server */
  if (write (sd3, msg3, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    } 
  
   /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd3, msg3, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("[client]Mesajul primit este: %s\n", msg3);

  close (sd3);

 // distrugere_container();
  gtk_widget_destroy(vbox);
 //  gtk_widget_destroy(grid);
  vbox = gtk_vbox_new(TRUE, 50);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  televizor = gtk_button_new_with_label("Televizor");
  lumini = gtk_button_new_with_label("Lumini");
  aspirator = gtk_button_new_with_label("Aspirator");
  quit = gtk_button_new_with_label("Quit");
  

  gtk_box_pack_start(GTK_BOX(vbox), televizor, TRUE, TRUE, 0);
 // g_signal_connect (televizor, "clicked", G_CALLBACK (TV), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), lumini, TRUE, TRUE, 0);
 // g_signal_connect (lumini, "clicked", G_CALLBACK (Lumini), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), aspirator, TRUE, TRUE, 0);
 // g_signal_connect (aspirator, "clicked", G_CALLBACK (Aspirator), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), quit, TRUE, TRUE, 0);
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_widget_destroy), window);


  gtk_widget_show_all(window);

}

void Aspirator(GtkWidget *widget, gpointer  data)
{
   GtkWidget *radio;
   GtkWidget *radio2;
   GtkWidget *entry1;
   GtkWidget *entry2;
   GtkWidget *label1;
   GtkWidget *button2;
   GtkWidget *button1;
   GtkWidget *button;

  /* portul de conectare la server*/
  // int port;
  //int sd3;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  //int msg3[100]=" ";		// mesajul trimis

  //stabilim portul serverului TV
  port = atoi ("2127");

  // cream socketul 
  if ((sd3 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  // umplem structura folosita pentru realizarea conexiunii cu serverul 
  // familia socket-ului 
  server.sin_family = AF_INET;
  // adresa IP a serverului 
  server.sin_addr.s_addr = inet_addr(IP);
  // portul de conectare 
  server.sin_port = htons (port);

   // ne conectam la server 
  if (connect (sd3, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
     bzero (msg3, 100);

   gtk_window_set_title(window, "Aspirator");  
  //distrugere_container();
   gtk_widget_destroy(vbox);

  vbox = gtk_vbox_new(FALSE,50);
  grid = gtk_grid_new();

   radio = gtk_radio_button_new_with_label( NULL, "Aspirator oprit");
 g_signal_connect(radio,"toggled", G_CALLBACK(toggled_func_a), (gpointer)"1");

  radio2 = gtk_radio_button_new_with_label (gtk_radio_button_get_group( GTK_RADIO_BUTTON(radio)),"Aspirator pornit");
  g_signal_connect(radio2,"toggled", G_CALLBACK(toggled_func_a), (gpointer)"2");

    gtk_box_pack_start(GTK_BOX(vbox), radio, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), radio2, FALSE, FALSE, 0);

  label1 = gtk_label_new("Arie de aspirare");
  gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
  //label2 = gtk_label_new("Lungime");
 // label3 = gtk_label_new("Latime");
  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  button1 = gtk_button_new_with_mnemonic("Modifica Lungimea");
  button2 = gtk_button_new_with_mnemonic("Modifica Latimea");
  gtk_grid_attach (GTK_GRID (grid), entry1, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button1, 1, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry2, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button2, 1, 1, 1, 1); 
  gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
 
  g_signal_connect (button1, "clicked", G_CALLBACK(button_entryL), entry1);
  g_signal_connect (button2, "clicked", G_CALLBACK(button_entryl), entry2);

   button = gtk_button_new_with_label ("Salveaza & Meniu");
  g_signal_connect (button, "clicked", G_CALLBACK (Meniu), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  gtk_widget_show_all (window); 


}

void MeniuL()
{

  GtkWidget *televizor;
  GtkWidget *lumini;
  GtkWidget *aspirator;
  GtkWidget *quit;

      /* citirea mesajului */
  // while(!quit)
  //bzero (msg2, 100);
  fflush (stdout);
  read (0, msg2, 100);
  
  /* trimiterea mesajului la server */
  if (write (sd2, msg2, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    } 
  
   /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd2, msg2, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("[client]Mesajul primit este: %s\n", msg2);

  close (sd2);

  //distrugere_container();
      gtk_widget_destroy(hbox);
      gtk_widget_destroy(vbox);

  vbox = gtk_vbox_new(TRUE, 50);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  televizor = gtk_button_new_with_label("Televizor");
  lumini = gtk_button_new_with_label("Lumini");
  aspirator = gtk_button_new_with_label("Aspirator");
  quit = gtk_button_new_with_label("Quit");
  

  gtk_box_pack_start(GTK_BOX(vbox), televizor, TRUE, TRUE, 0);
 // g_signal_connect (televizor, "clicked", G_CALLBACK (TV), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), lumini, TRUE, TRUE, 0);
 // g_signal_connect (lumini, "clicked", G_CALLBACK (Lumini), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), aspirator, TRUE, TRUE, 0);
  g_signal_connect (aspirator, "clicked", G_CALLBACK (Aspirator), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), quit, TRUE, TRUE, 0);
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_widget_destroy), window);


  gtk_widget_show_all(window);

}

void Lumini(GtkWidget *widget, gpointer  data)
{
   GtkWidget *radio;
   GtkWidget *radio2;
   GtkWidget *combo;
   GtkWidget *label;
   GtkWidget *label1;
   GtkWidget *hscale;
   GtkWidget *button2;
   
   /* portul de conectare la server*/
   //int port;
  //int sd2;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  //int msg2[100]=" ";		// mesajul trimis

  //stabilim portul serverului TV
  port = atoi ("2126");

  // cream socketul 
  if ((sd2 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  // umplem structura folosita pentru realizarea conexiunii cu serverul 
  // familia socket-ului 
  server.sin_family = AF_INET;
  // adresa IP a serverului 
  server.sin_addr.s_addr = inet_addr(IP);
  // portul de conectare 
  server.sin_port = htons (port);

   // ne conectam la server 
  if (connect (sd2, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

     bzero (msg2, 100);

    gtk_window_set_title(window, "Lumini");  
  //distrugere_container();
   gtk_widget_destroy(vbox);

  vbox = gtk_vbox_new(FALSE,50);
  hbox = gtk_hbox_new(FALSE, 50);

  gtk_container_add (GTK_CONTAINER (window), vbox);

   radio = gtk_radio_button_new_with_label( NULL, "Lumini oprite");
 g_signal_connect(radio,"toggled", G_CALLBACK(toggled_func_l), (gpointer)"1");

  radio2 = gtk_radio_button_new_with_label (gtk_radio_button_get_group( GTK_RADIO_BUTTON(radio)),"Lumini pornite");
  g_signal_connect(radio2,"toggled", G_CALLBACK(toggled_func_l), (gpointer)"2");

   gtk_box_pack_start(GTK_BOX(vbox), radio, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), radio2, FALSE, FALSE, 0);

  label = gtk_label_new("Culoare");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);


  combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo),NULL, "Alb Cald");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo),NULL, "Alb Rece");
  g_signal_connect(combo, "changed", G_CALLBACK(combo_selected), NULL);

 gtk_box_pack_start(GTK_BOX(hbox), combo, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  hscale = gtk_hscale_new_with_range(1, 3, 1);
  gtk_scale_set_draw_value(GTK_SCALE(hscale), TRUE);
  gtk_widget_set_size_request(hscale, 150, -1);
  label1 = gtk_label_new("Intensitate:\
  1 = Mare\
  2 = Medie\
  3 = Mica");
  //gtk_misc_set_alignment(GTK_MISC(label), 0.0, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hscale, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);

  g_signal_connect(hscale, "value-changed", G_CALLBACK(value_changedl),NULL); 

  button2 = gtk_button_new_with_label ("Salveaza & Meniu");
  g_signal_connect (button2, "clicked", G_CALLBACK (MeniuL), NULL);

  gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);


  gtk_widget_show_all (window); 

}

void MeniuT2()
{
 
  GtkWidget *televizor;
  GtkWidget *lumini;
  GtkWidget *aspirator;
  GtkWidget *quit;
  

    /* citirea mesajului */
  // while(!quit)
  //bzero (msg, 100);
  fflush (stdout);
  read (0, msg, 100);
  
  /* trimiterea mesajului la server */
  if (write (sd, msg, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    } 
  
   /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, msg, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("[client]Mesajul primit este: %s\n", msg);
  /*if(strcmp(msg,"Dispozitiv inactiv")==0)
        {
          g_signal_connect(televizor, "clicked", G_CALLBACK(show_error), (gpointer)window); 
          g_signal_connect(lumini, "clicked", G_CALLBACK(show_error),  (gpointer)window); 
          g_signal_connect(aspirator, "clicked", G_CALLBACK(show_error), (gpointer)window); 
        }*/
  close (sd);

  //distrugere_container();
   gtk_widget_destroy(hbox);
   gtk_widget_destroy(vbox);
  vbox = gtk_vbox_new(TRUE, 50);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  televizor = gtk_button_new_with_label("Televizor");
  lumini = gtk_button_new_with_label("Lumini");
  aspirator = gtk_button_new_with_label("Aspirator");
  quit = gtk_button_new_with_label("Quit");
  

  gtk_box_pack_start(GTK_BOX(vbox), televizor, TRUE, TRUE, 0);
 // g_signal_connect (televizor, "clicked", G_CALLBACK (TV2), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), lumini, TRUE, TRUE, 0);
  g_signal_connect (lumini, "clicked", G_CALLBACK (Lumini), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), aspirator, TRUE, TRUE, 0);
  g_signal_connect (aspirator, "clicked", G_CALLBACK (Aspirator), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), quit, TRUE, TRUE, 0);
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_widget_destroy), window);


  gtk_widget_show_all(window);

}

void TV2(GtkWidget *widget, gpointer  data)
{

   GtkWidget *button;
   GtkWidget *hscale;
   GtkWidget *radio;
   GtkWidget *radio2;
   GtkWidget *label;
   GtkWidget *label1;
   GtkWidget *entry1;
   GtkWidget *buttone;

  /* portul de conectare la server*/
  //int port;
  //int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  //char msg[100]=" ";		// mesajul trimis

  //stabilim portul serverului TV
  port = atoi ("2125");

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(IP);
  /* portul de conectare */
  server.sin_port = htons (port);

   /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
     bzero (msg, 100);

  gtk_window_set_title(window, "TV");  
 // distrugere_container();
  gtk_widget_destroy(vbox);
  
  vbox = gtk_vbox_new(FALSE,50);
  hbox = gtk_hbox_new(0, 10);

  gtk_container_add (GTK_CONTAINER (window), vbox);

   radio = gtk_radio_button_new_with_label( NULL, "Oprit");
  //gtk_grid_attach(GTK_GRID(grid),radio,0,0,1,1);
 g_signal_connect(radio,"toggled", G_CALLBACK(toggled_func), (gpointer)"1");

  radio2 = gtk_radio_button_new_with_label (gtk_radio_button_get_group( GTK_RADIO_BUTTON(radio)),"Pornit");
   //  gtk_grid_attach(GTK_GRID(grid),radio2,1,0,1,1);
  g_signal_connect(radio2,"toggled", G_CALLBACK(toggled_func), (gpointer)"2");


  hscale = gtk_hscale_new_with_range(0, 100, 1);
  gtk_scale_set_draw_value(GTK_SCALE(hscale), TRUE);
  gtk_widget_set_size_request(hscale, 150, -1);
  label = gtk_label_new("Volum");
  //gtk_misc_set_alignment(GTK_MISC(label), 0.0, 1);

  label1 = gtk_label_new("Canal");
  entry1 = gtk_entry_new();
  buttone = gtk_button_new_with_mnemonic("Modifica");
  gtk_box_pack_start (GTK_BOX(hbox), label1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX(hbox), entry1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX(hbox), buttone, FALSE, FALSE, 0); 


   button = gtk_button_new_with_label ("Salveaza & Meniu");
  g_signal_connect (button, "clicked", G_CALLBACK (MeniuT2), NULL);

  gtk_box_pack_start(GTK_BOX(vbox), radio, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), radio2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hscale, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
  g_signal_connect(hscale, "value-changed", G_CALLBACK(value_changed),NULL);     
  g_signal_connect (buttone, "clicked", G_CALLBACK(button_entryc), entry1);
 gtk_widget_show_all (window);


}


void MeniuT()
{
 
  GtkWidget *televizor;
  GtkWidget *lumini;
  GtkWidget *aspirator;
  GtkWidget *quit;
  

    /* citirea mesajului */
  // while(!quit)
  //bzero (msg, 100);
  fflush (stdout);
  read (0, msg, 100);
  
  /* trimiterea mesajului la server */
  if (write (sd, msg, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    } 
  
   /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, msg, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("[client]Mesajul primit este: %s\n", msg);
  /*if(strcmp(msg,"Dispozitiv inactiv")==0)
        {
          g_signal_connect(televizor, "clicked", G_CALLBACK(show_error), (gpointer)window); 
          g_signal_connect(lumini, "clicked", G_CALLBACK(show_error), (gpointer)window); 
          g_signal_connect(aspirator, "clicked", G_CALLBACK(show_error),(gpointer) window); 
        }*/
  close (sd);

  //distrugere_container();
   gtk_widget_destroy(hbox);
   gtk_widget_destroy(vbox);
  vbox = gtk_vbox_new(TRUE, 50);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  televizor = gtk_button_new_with_label("Televizor");
  lumini = gtk_button_new_with_label("Lumini");
  aspirator = gtk_button_new_with_label("Aspirator");
  quit = gtk_button_new_with_label("Quit");
  

  gtk_box_pack_start(GTK_BOX(vbox), televizor, TRUE, TRUE, 0);
  g_signal_connect (televizor, "clicked", G_CALLBACK (TV2), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), lumini, TRUE, TRUE, 0);
  g_signal_connect (lumini, "clicked", G_CALLBACK (Lumini), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), aspirator, TRUE, TRUE, 0);
  g_signal_connect (aspirator, "clicked", G_CALLBACK (Aspirator), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), quit, TRUE, TRUE, 0);
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_widget_destroy), window);


  gtk_widget_show_all(window);

}


void TV(GtkWidget *widget, gpointer  data)
{

   GtkWidget *button;
   GtkWidget *hscale;
   GtkWidget *radio;
   GtkWidget *radio2;
   GtkWidget *label;
   GtkWidget *label1;
   GtkWidget *entry1;
   GtkWidget *buttone;

  /* portul de conectare la server*/
  //int port;
  //int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  //char msg[100]=" ";		// mesajul trimis

  //stabilim portul serverului TV
  port = atoi ("2125");

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(IP);
  /* portul de conectare */
  server.sin_port = htons (port);

   /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

     bzero (msg, 100);

  gtk_window_set_title(window, "TV");  
 // distrugere_container();
  gtk_widget_destroy(vbox);
  
  vbox = gtk_vbox_new(FALSE,50);
  hbox = gtk_hbox_new(0, 10);

  gtk_container_add (GTK_CONTAINER (window), vbox);

   radio = gtk_radio_button_new_with_label( NULL, "Oprit");
  //gtk_grid_attach(GTK_GRID(grid),radio,0,0,1,1);
 g_signal_connect(radio,"toggled", G_CALLBACK(toggled_func), (gpointer)"1");

  radio2 = gtk_radio_button_new_with_label (gtk_radio_button_get_group( GTK_RADIO_BUTTON(radio)),"Pornit");
   //  gtk_grid_attach(GTK_GRID(grid),radio2,1,0,1,1);
  g_signal_connect(radio2,"toggled", G_CALLBACK(toggled_func), (gpointer)"2");


  hscale = gtk_hscale_new_with_range(0, 100, 1);
  gtk_scale_set_draw_value(GTK_SCALE(hscale), TRUE);
  gtk_widget_set_size_request(hscale, 150, -1);
  label = gtk_label_new("Volum");
  //gtk_misc_set_alignment(GTK_MISC(label), 0.0, 1);

  label1 = gtk_label_new("Canal");
  entry1 = gtk_entry_new();
  buttone = gtk_button_new_with_mnemonic("Modifica");
  gtk_box_pack_start (GTK_BOX(hbox), label1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX(hbox), entry1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX(hbox), buttone, FALSE, FALSE, 0); 


   button = gtk_button_new_with_label ("Salveaza & Meniu");
  g_signal_connect (button, "clicked", G_CALLBACK (MeniuT), NULL);

  gtk_box_pack_start(GTK_BOX(vbox), radio, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), radio2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hscale, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
  g_signal_connect(hscale, "value-changed", G_CALLBACK(value_changed),NULL);     
  g_signal_connect (buttone, "clicked", G_CALLBACK(button_entryc), entry1);
 gtk_widget_show_all (window);


}


 void activate(GtkApplication *app, gpointer data)
{

  GtkWidget *televizor;
  GtkWidget *lumini;
  GtkWidget *aspirator;
  GtkWidget *quit;
 
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "REMOTE");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
  gtk_container_set_border_width (GTK_CONTAINER (window), 50);

  vbox = gtk_vbox_new(TRUE, 50);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  televizor = gtk_button_new_with_label("Televizor");
  lumini = gtk_button_new_with_label("Lumini");
  aspirator = gtk_button_new_with_label("Aspirator");
  quit = gtk_button_new_with_label("Quit");
  
  gtk_box_pack_start(GTK_BOX(vbox), televizor, TRUE, TRUE, 0);
  g_signal_connect (televizor, "clicked", G_CALLBACK (TV), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), lumini, TRUE, TRUE, 0);
  g_signal_connect (lumini, "clicked", G_CALLBACK (Lumini), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), aspirator, TRUE, TRUE, 0);
  g_signal_connect (aspirator, "clicked", G_CALLBACK (Aspirator), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), quit, TRUE, TRUE, 0);
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_widget_destroy), window);

  //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), window);

  gtk_widget_show_all(window);


}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("gtk.client", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;

}

