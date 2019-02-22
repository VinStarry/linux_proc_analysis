#include <stdio.h>
#include <gtk/gtk.h>
#include "sys_page.h"
#include "mem_page.h"
#include "ps_page.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HIGHT 1200
#define LINE_CHART_WIDTH 1500
#define LINE_CHART_HEIGHT 160
#define ROW_NUMBER_MAX 400

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);
void switch_page_handle(GtkNotebook *notebook, gpointer page, guint page_num, gpointer data);
void *thread_func(void *arg);

bool endSignal = true;
GThread *th;

GtkWidget *vbox, *second_vbox, *third_vbox;
lb_list *lb_head;

int main(int argc, char *argv[]) {
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
    second_vbox = gtk_vbox_new(FALSE, 5);
    GtkWidget *overall_frame_title = gtk_label_new("");
    gtk_label_set_markup(
            GTK_LABEL(overall_frame_title),
            "<span foreground='brown' font_desc='24'>Overall Information</span>");

    GtkWidget *overall_frame = gtk_frame_new("");
    gtk_frame_set_label_widget(GTK_FRAME(overall_frame), overall_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(overall_frame), 20);
    gtk_box_pack_start(GTK_BOX(second_vbox), overall_frame, false, false, 5);

    GtkWidget *cpu_usage = gtk_label_new("CPU usage: 0.0%");
    GtkWidget *usage_info_vbox = gtk_vbox_new(false, 0);

    init_cpu_array();
    GtkWidget *cpu_usage_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(cpu_usage_drawing_area, LINE_CHART_WIDTH, LINE_CHART_HEIGHT);

    gtk_box_pack_start(GTK_BOX(usage_info_vbox), cpu_usage_drawing_area, false, false, 5);
    gtk_box_pack_start(GTK_BOX(usage_info_vbox), cpu_usage, false, false, 5);

    g_timeout_add(1000, update_cpu_usage, cpu_usage);
    g_signal_connect(G_OBJECT(cpu_usage_drawing_area), "expose_event", G_CALLBACK(usage_cpu_draw), NULL);

    GtkWidget *mem_usage = gtk_label_new("Memory usage: 0.0%");

    GtkWidget *mem_usage_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(mem_usage_drawing_area, LINE_CHART_WIDTH, LINE_CHART_HEIGHT);

    gtk_box_pack_start(GTK_BOX(usage_info_vbox), mem_usage_drawing_area, false, false, 5);
    gtk_box_pack_start(GTK_BOX(usage_info_vbox), mem_usage, false, false, 5);

    gtk_container_add(GTK_CONTAINER(overall_frame), usage_info_vbox);

    g_timeout_add(1000, update_mem_usage, mem_usage);
    g_signal_connect(G_OBJECT(mem_usage_drawing_area), "expose_event", G_CALLBACK(usage_mem_draw), NULL);

    GtkWidget *ps_frame_title = gtk_label_new("");
//    GtkWidget *proc_frame =  get_proc_info();
    GtkWidget *proc_frame = gtk_frame_new("");
    gtk_label_set_markup(
            GTK_LABEL(ps_frame_title),
            "<span foreground='brown' font_desc='24'>Process information</span>");
    gtk_frame_set_label_widget(GTK_FRAME(proc_frame), ps_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(proc_frame), 20);

    GtkWidget *table = gtk_table_new(ROW_NUMBER_MAX, 7, false);
    GtkWidget *tbscrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(proc_frame),tbscrolled);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(tbscrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

    gtk_scrolled_window_add_with_viewport (
            GTK_SCROLLED_WINDOW (tbscrolled), table);

    GtkWidget *pid_button = gtk_button_new_with_label("PID"),
              *user_button = gtk_button_new_with_label("USER"),
              *mem_per_button = gtk_button_new_with_label("%MEM"),
              *cpu_per_button = gtk_button_new_with_label("%CPU"),
              *vsz_button = gtk_button_new_with_label("VSZ"),
              *rss_button = gtk_button_new_with_label("RSS"),
              *state_button = gtk_button_new_with_label("STATE"),
              *cmdline_button = gtk_button_new_with_label("COMMAND");

    gtk_table_attach(GTK_TABLE(table), pid_button, 0, 1, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), user_button, 1, 2, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), mem_per_button, 2, 3, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), cpu_per_button, 3, 4, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), vsz_button, 4, 5, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), rss_button, 5, 6, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), state_button, 6, 7, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), cmdline_button, 7, 8, (guint)0, (guint)1, GTK_EXPAND, GTK_EXPAND, 0, 0);

    init_proc_table(table);

    gtk_container_add(GTK_CONTAINER(second_vbox), proc_frame);

    g_signal_connect(G_OBJECT(pid_button), "pressed", G_CALLBACK(sort_by_pid), NULL);
    g_signal_connect(G_OBJECT(user_button), "pressed", G_CALLBACK(sort_by_user), NULL);
    g_signal_connect(G_OBJECT(mem_per_button), "pressed", G_CALLBACK(sort_by_memper), NULL);
    g_signal_connect(G_OBJECT(cpu_per_button), "pressed", G_CALLBACK(sort_by_cpuper), NULL);
    g_signal_connect(G_OBJECT(vsz_button), "pressed", G_CALLBACK(sort_by_vsz), NULL);
    g_signal_connect(G_OBJECT(rss_button), "pressed", G_CALLBACK(sort_by_rss), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), second_vbox, label);

    g_timeout_add(5000, update_proc_table, table);

    // page three
    label = gtk_label_new("Memory");
    third_vbox = gtk_vbox_new(TRUE, 5);

    GtkWidget *mem_frame_title = gtk_label_new("");
    gtk_label_set_markup(
            GTK_LABEL(mem_frame_title),
            "<span foreground='brown' font_desc='24'>Memory specific</span>");

    GtkWidget *mem_frame = gtk_frame_new("");
    gtk_frame_set_label_widget(GTK_FRAME(mem_frame), mem_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(mem_frame), 20);
    gtk_container_add(GTK_CONTAINER(third_vbox), mem_frame);

    GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(mem_frame),scrolled);
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
    free_mps();
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