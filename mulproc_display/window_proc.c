//
// Created by vinstarry on 2/9/19.
//

#include "window_proc.h"

#define BUF_LEN 50
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 200
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

typedef struct _re_data {
    GtkWidget *label;
    int rank;
}g_data;

bool endSignal = false;
GtkWidget *content = NULL;
int proc_num = 0;
long all1 = 0, all2 = 0, idle1 = 0, idle2 = 0;

void wind_proc(int argc, char *argv[], int rank)
{
    gtk_init(&argc, &argv);
    // main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    g_signal_connect(window,"destroy",G_CALLBACK(destroy_handle),NULL);

    char *title = title_name(rank);
    proc_num = rank;
    // layouts
    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
    GtkWidget *button_box = gtk_hbox_new(FALSE, 0);
    GtkWidget *label = gtk_label_new(title);
    content = gtk_label_new("display here");

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(button_box), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(label), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(content), FALSE, FALSE, 0);

    // two butttons
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *stop_button = gtk_button_new_with_label("Stop");

    gtk_widget_set_size_request(start_button, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_widget_set_size_request(stop_button, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(button_box), GTK_WIDGET(start_button), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), GTK_WIDGET(stop_button), TRUE, TRUE, 0);

    g_signal_connect(start_button,"clicked",G_CALLBACK(start_display), NULL);
    g_signal_connect(stop_button,"clicked",G_CALLBACK(stop_display), NULL);

    gtk_widget_show_all(window);

    gdk_threads_init();

    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
}

gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = true;
    gtk_main_quit();
    return FALSE;
}

char *title_name(int rank) {
    char *title = NULL;
    switch (rank) {
        case 0:
            title = "System Time";
            break;
        case 1:
            title = "CPU Usage";
            break;
        case 2:
            title = "Adder";
            break;
        default:
            title = "ERROR";
            break;
    }
    return title;
}

char *get_system_time() {
    char *time_text = (char *)malloc(sizeof(char) * BUF_LEN);
    time_t local_time;
    struct tm *lt;
    time(&local_time);
    lt = localtime (&local_time);
    int len = sprintf(time_text, "%d/%d/%d %d:%d:%d\n", lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    time_text[len] = '\0';
    return time_text;
}

char *get_adder_info(int *number, int *sum) {
    char *adder_text = (char *)malloc(sizeof(char) * BUF_LEN);
    sprintf(adder_text, "%d + %d = %d\n", *sum, *number, *sum + *number);
    *sum = *sum + *number;
    *number = *number + 1;
    return adder_text;
}

char *get_cpu_usager(long all1, long all2, long idle1, long idle2) {
    char *usage_text = (char *)malloc(sizeof(char) * BUF_LEN);
    float usage;

    char buf[128] = {'\0'};
    char cpu[5];
    long int user,nice,sys,idle,iowait,irq,softirq;

    FILE * fp = fopen("/proc/stat","r");
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

    all2 = user+nice+sys+idle+iowait+irq+softirq;
    idle2 = idle;
    fclose(fp);

    usage = (float)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ;

    sprintf(usage_text, "cpu usage = %.2f%%\n",usage);
    return usage_text;
}

void start_display(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = false;
    GThread *th;
    switch (proc_num) {
        case 0:
            th = g_thread_new ("a", thread_func1, NULL);
            break;
        case 1:
            th = g_thread_new ("a", thread_func2, NULL);
            break;
        case 2:
            th = g_thread_new ("c", thread_func3, NULL);
            break;
        default:
            break;
    }
}

void stop_display(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = true;
}

void *thread_func1(void *arg) {
    while(!endSignal) {
        sleep(1);
        gdk_threads_enter();
        gtk_label_set_text(GTK_LABEL(content), get_system_time());
        gdk_threads_leave();
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while(!endSignal) {
        sleep(1);
        char buf[128] = {'\0'};
        char cpu[5];
        long int user,nice,sys,idle,iowait,irq,softirq;

        FILE * fp = fopen("/proc/stat","r");
        fgets(buf,sizeof(buf),fp);
        sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

        all1 = user+nice+sys+idle+iowait+irq+softirq;
        idle1 = idle;
        fclose(fp);
        sleep(1);
        gdk_threads_enter();
        gtk_label_set_text(GTK_LABEL(content), get_cpu_usager(all1, all2, idle1, idle2));
        gdk_threads_leave();
    }
    return NULL;
}

void *thread_func3(void *arg) {
    int num = 0, sum = 0;
    while(!endSignal) {
        sleep(3);
        gdk_threads_enter();
        gtk_label_set_text(GTK_LABEL(content), get_adder_info(&num, &sum));
        gdk_threads_leave();
    }
    return NULL;
}