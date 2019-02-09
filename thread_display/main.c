#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "window_functions.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 50

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);
void start_display(GtkWidget *self, GdkEvent *event, gpointer data);
void stop_display(GtkWidget *self, GdkEvent *event, gpointer data);
void *thread1_func(void *arg);
void *thread2_func(void *arg);
void *thread3_func(void *arg);

bool endSignal = TRUE;
GtkWidget *date, *cpu, *sum;
long all1 = 0, all2 = 0, idle1 = 0, idle2 = 0;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    g_signal_connect(window,"destroy",G_CALLBACK(destroy_handle),NULL);

    // layouts
    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
    GtkWidget *button_box = gtk_hbox_new(FALSE, 0);
    GtkWidget *display_box = gtk_hbox_new(FALSE, 10);
    GtkWidget *systime_vbox = gtk_vbox_new(FALSE, 10);
    GtkWidget *monitor_vbox = gtk_vbox_new(FALSE, 10);
    GtkWidget *adder_vbox = gtk_vbox_new(FALSE, 10);

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(button_box), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(display_box), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(display_box), GTK_WIDGET(systime_vbox), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(display_box), GTK_WIDGET(monitor_vbox), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(display_box), GTK_WIDGET(adder_vbox), TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // two butttons
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *stop_button = gtk_button_new_with_label("Stop");

    gtk_widget_set_size_request(start_button, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_widget_set_size_request(stop_button, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(button_box), GTK_WIDGET(start_button), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), GTK_WIDGET(stop_button), TRUE, TRUE, 0);

    g_signal_connect(start_button,"clicked",G_CALLBACK(start_display),NULL);
    g_signal_connect(stop_button,"clicked",G_CALLBACK(stop_display),NULL);

    // three labels to display
    GtkWidget *label_time = gtk_label_new("System time");
    GtkWidget *label_monitor = gtk_label_new("CPU monitor");
    GtkWidget *label_adder = gtk_label_new("Adder");
    date = gtk_label_new("in here");
    cpu = gtk_label_new("in here");
    sum = gtk_label_new("in here");

    gtk_box_pack_start(GTK_BOX(systime_vbox), GTK_WIDGET(label_time), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(monitor_vbox), GTK_WIDGET(label_monitor), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(adder_vbox), GTK_WIDGET(label_adder), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(systime_vbox), GTK_WIDGET(date), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(monitor_vbox), GTK_WIDGET(cpu), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(adder_vbox), GTK_WIDGET(sum), TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gdk_threads_init();

    gdk_threads_enter();
    gtk_main ();
    gdk_threads_leave();
    return 0;
}

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

void start_display(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = false;
    GThread *th1, *th2, *th3;

    th1 = g_thread_new ("a", thread1_func, NULL);
    th2 = g_thread_new ("b", thread2_func, NULL);
    th3 = g_thread_new ("c", thread3_func, NULL);
}

void stop_display(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = TRUE;
}

void *thread1_func(void *arg) {
    char buf[128] = {'\0'};
    char cpu[5];
    long int user,nice,sys,idle,iowait,irq,softirq;

    while (!endSignal) {
        sleep(1);
        gdk_threads_enter();
        FILE * fp = fopen("/proc/stat","r");
        fgets(buf,sizeof(buf),fp);
        sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
        all1 = user+nice+sys+idle+iowait+irq+softirq;
        idle1 = idle;
        fclose(fp);

        char *text = get_system_time();
        gtk_label_set_text(GTK_LABEL(date), text);
        gdk_threads_leave();
    }
    return NULL;
}

void *thread2_func(void *arg) {
    while (!endSignal) {
        sleep(2);
        gdk_threads_enter();
        char *text = get_cpu_usager(all1, all2, idle1, idle2);
        gtk_label_set_text(GTK_LABEL(cpu), text);
        gdk_threads_leave();
    }
    return NULL;
}

void *thread3_func(void *arg) {
    int num = 0, local_sum = 0;
    while (!endSignal) {
        sleep(3);
        if (num > 100)
            break;
        gdk_threads_enter();
        char *text = get_adder_info(&num, &local_sum);
        gtk_label_set_text(GTK_LABEL(sum), text);
        gdk_threads_leave();
    }
    return NULL;
}
