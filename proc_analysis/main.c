#include <stdio.h>
#include <gtk/gtk.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HIGHT 800

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);
void deal_switch_page(GtkNotebook *notebook, gpointer page, guint page_num, gpointer data);

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    g_signal_connect(window,"destroy",G_CALLBACK(destroy_handle),NULL);

    // notebook
    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

    // layouts
    GtkWidget *vbox = gtk_vbox_new(TRUE, 5);

    // page one
    GtkWidget *label = gtk_label_new("System");

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);

    //7.第二个页面
    label = gtk_label_new("Process");
    vbox = gtk_vbox_new(TRUE, 5);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);

    label = gtk_label_new("Memory");
    vbox = gtk_vbox_new(TRUE, 5);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);

    g_signal_connect(notebook, "switch-page", G_CALLBACK(deal_switch_page), vbox);

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

void deal_switch_page(GtkNotebook *notebook, gpointer page, guint page_num, gpointer data)
{
    printf("我是第%d个页面\n", page_num+1);
    GtkWidget *vbox = data;
    if (page_num == 0) {
        printf("hello\n");
        GtkWidget *label = gtk_label_new("hello");
        gtk_container_add(GTK_CONTAINER(vbox), label);
    }
}