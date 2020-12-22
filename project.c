//for compile:  gcc `pkg-config gtk+-3.0 --cflags` project.c -o project `pkg-config gtk+-3.0 --libs`

#include <net/if.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#define _XOPEN_SOURCE 600 
#include <libgen.h>


void clicked_button(GtkWidget *widget, gpointer data,char * argv[]) 
{
system("figlet PODCAST");
system("gnome-terminal");
system("gnome-terminal");
system("gnome-terminal");
printf("\nHello there, Welcome to the PODCAST. You can type this command --> ./moreinfo_p in any of these 3 command window.\n");
}
void clicked_button1(GtkWidget *widget, gpointer data,char * argv[]) 
{
system("figlet MOVIE_MANIA");
system("gnome-terminal");
system("gnome-terminal");
system("gnome-terminal");
printf("\nHello there, Welcome to the MOVIE_MANIA. You can type this command --> ./moreinfo_m in any of these 3 command window to get more info.\n");
}
int main(int argc, char * argv[])
{
    // Initializing Variables for GUI main window
    GtkWidget *window;
    GtkWidget *halign;
    GtkWidget *vbox;
    GtkWidget *btn;
    GtkWidget *btn1;
    argc=1;
    gtk_init(&argc, &argv);
    // Creation of the Station List GUI Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Welcome to the Podcast Stories");
    gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    vbox=gtk_box_new(TRUE,1);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    //Assigning labels to the 4 buttons corresponding to 4 options given to user
    btn = gtk_button_new_with_label("Play Podcast");
    btn1 = gtk_button_new_with_label("Play Video");
    
    // Setting button's length and width
    gtk_widget_set_size_request(btn, 70, 30);
    gtk_widget_set_size_request(btn1, 70, 30);
    

    gtk_box_pack_start(GTK_BOX(vbox), btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn1, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(clicked_button),argv);
    g_signal_connect(G_OBJECT(btn1), "clicked", G_CALLBACK(clicked_button1),argv);

    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), G_OBJECT(window));
    
    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}
