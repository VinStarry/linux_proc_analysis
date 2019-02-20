#include <stdio.h>
#include <gtk/gtk.h>
#include "functions.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HIGHT 1200

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);
void switch_page_handle(GtkNotebook *notebook, gpointer page, guint page_num, gpointer data);
void *thread_func(void *arg);

bool endSignal = true;
GThread *th;

GtkWidget *vbox, *second_vbox, *third_vbox;
lb_list *lb_head;

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
    vbox = system_general_info();

    // page one
    GtkWidget *label = gtk_label_new("System");

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);

    // page two
    label = gtk_label_new("Process");
    second_vbox = get_proc_info();

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), second_vbox, label);

    // page three
    label = gtk_label_new("Memory");
    third_vbox = gtk_vbox_new(TRUE, 5);
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(third_vbox),scrolled);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    lb_head = (lb_list *)malloc(sizeof(lb_head));
    lb_head->next = NULL;
    unsigned int lines = memory_info_lable_attach(lb_head);
    GtkWidget* pHtable = gtk_table_new(lines + 1, 2, false);
    gtk_scrolled_window_add_with_viewport (
            GTK_SCROLLED_WINDOW (scrolled), pHtable);
    attach_memory_info(pHtable, lb_head);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), third_vbox, label);

    g_signal_connect(notebook, "switch-page", G_CALLBACK(switch_page_handle), NULL);

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),1);

    gtk_widget_show_all(window);

    gdk_threads_init();

    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();

    return 0;
}

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = true;
    gtk_main_quit();
    return FALSE;
}

void switch_page_handle(GtkNotebook *notebook, gpointer page, guint page_num, gpointer data)
{
    printf("this is page no. %d\n", page_num + 1);
    switch (page_num) {
        case 0:
            endSignal = true;
            break;
        case 1:
            endSignal = true;
            break;
        case 2:
            endSignal = false;
            th = g_thread_new("a", thread_func, NULL);
            break;
        default:
            break;
    }
}

void *thread_func(void *arg) {
    while (!endSignal) {
        sleep(2);
        gdk_threads_enter();
        refresh_memory_info(lb_head);
        gdk_threads_leave();
    }
    return NULL;
}