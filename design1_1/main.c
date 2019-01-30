#include <stdio.h>
#include <gtk/gtk.h>
#include "window_functions.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 50

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *button_container = gtk_fixed_new();
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *stop_button = gtk_button_new_with_label("Stop");
    GtkWidget *box_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *label_time = gtk_label_new("temp");

    gtk_container_add(GTK_CONTAINER(box_time), label_time);

    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    gtk_widget_set_size_request(button_container, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_fixed_put(GTK_FIXED(button_container), start_button, 0, 0);
    gtk_fixed_put(GTK_FIXED(button_container), stop_button, BUTTON_WIDTH, 0);
    gtk_fixed_put(GTK_FIXED(button_container), box_time, 500, 0);
    gtk_fixed_move(GTK_FIXED(button_container), start_button, 0, 0);

    gtk_widget_set_size_request(start_button, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_widget_set_size_request(stop_button, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_widget_set_size_request(label_time, 500, BUTTON_HEIGHT);

    gtk_container_add(GTK_CONTAINER(window), button_container);

    g_signal_connect(window,"destroy",G_CALLBACK(destroy_handle),NULL);

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data){
    gtk_main_quit();
    return FALSE;
}