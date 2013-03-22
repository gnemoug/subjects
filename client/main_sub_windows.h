#ifndef __MAIN_SUB_WINDOWS_H__
#define __MAIN_SUB_WINDOWS_H__

#include <gtk/gtk.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int client( char buffer[1024] );
void on_show1 (GtkButton* button, gpointer data);
GtkWidget* create_main_window ();
void on_show2 (GtkButton* button,gpointer data);
void file();

#endif
