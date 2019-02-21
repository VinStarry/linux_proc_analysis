//
// Created by vinstarry on 2/9/19.
//

#include "window_proc.h"

#define BUF_LEN 50
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 200
#define HUGE_WINDOW_WIDTH 1000
#define HUGE_WINDOW_HEIGHT 600
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define FSTAB_INFO_COL 6
#define FSTAB_INFO_ROW 20
#define MAX_SPEC 2000
#define MAX_LINE_C 200

typedef struct _re_data {
    GtkWidget *label;
    int rank;
}g_data;

bool endSignal = false;
GtkWidget *content = NULL;
int proc_num = 0;
long all1 = 0, all2 = 0, idle1 = 0, idle2 = 0;

cross_linked_list *table_head = NULL;
GtkWidget *label_output = NULL;

void wind_proc(int argc, char *argv[], int rank) {
    gtk_init(&argc, &argv);
    // main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // window no.1 is a huge window
    if (rank != 1)
        gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    else
        gtk_widget_set_size_request(window, HUGE_WINDOW_WIDTH, HUGE_WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    g_signal_connect(window,"destroy",G_CALLBACK(destroy_handle),NULL);

    char *title = title_name(rank);
    proc_num = rank;
    // layouts
    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
    GtkWidget *button_box = gtk_hbox_new(FALSE, 0);

    GtkWidget *label = gtk_label_new(title);

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(button_box), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(label), FALSE, FALSE, 0);

    if (rank != 1) {
        content = gtk_label_new("display here");
        gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(content), FALSE, FALSE, 0);
    }
    else {
        GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
        gtk_container_add(GTK_CONTAINER(vbox),scrolled);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

        table_head = (cross_linked_list *)malloc(sizeof(cross_linked_list));

        GtkWidget* pHtable = gtk_table_new(FSTAB_INFO_ROW, FSTAB_INFO_COL, false);
        gtk_table_set_col_spacings(GTK_TABLE(pHtable), 10);
        gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled), pHtable);

        init_table(pHtable, table_head);

        GtkWidget *specific_content = gtk_label_new("specific content");
        gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(specific_content), FALSE, FALSE, 0);

        label_output = gtk_label_new("");
        GtkWidget *scrolled2 = gtk_scrolled_window_new(NULL,NULL);
        gtk_container_add(GTK_CONTAINER(vbox), scrolled2);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled2), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

        gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled2), label_output);
    }

    gtk_container_add(GTK_CONTAINER(window), vbox);

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
            title = "Fstab Info";
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

//char *get_cpu_usager(long all1, long all2, long idle1, long idle2) {
//    char *usage_text = (char *)malloc(sizeof(char) * BUF_LEN);
//    float usage;
//
//    char buf[128] = {'\0'};
//    char cpu[5];
//    long int user,nice,sys,idle,iowait,irq,softirq;
//
//    FILE * fp = fopen("/proc/stat","r");
//    fgets(buf,sizeof(buf),fp);
//    sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
//
//    all2 = user+nice+sys+idle+iowait+irq+softirq;
//    idle2 = idle;
//    fclose(fp);
//
//    usage = (float)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ;
//
//    sprintf(usage_text, "cpu usage = %.2f%%\n",usage);
//    return usage_text;
//}

char *refresh_content(int line_no) {
    FILE *fp_fstab = NULL;
    char *message = (char *)malloc(sizeof(char) * MAX_LINE_C);
    fp_fstab = fopen("/etc/fstab", "r");
    if (fp_fstab == NULL)
        strcpy(message, "Cannot open /etc/fstab.\n");
    else {
        for (int i = 0; i <= line_no; i++) {
            if (fgets(message, MAX_LINE_C, fp_fstab) == NULL)
                strcpy(message, "");
        }
    }
    return message;
}

void start_display(GtkWidget *self, GdkEvent *event, gpointer data) {
    endSignal = false;
    GThread *th;
    switch (proc_num) {
        case 0:
            th = g_thread_new ("a", thread_func1, NULL);
            break;
        case 1:
            th = g_thread_new ("b", thread_func2, NULL);
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
    char specific[MAX_SPEC] = {'\0'};
    memset(specific, 0, MAX_SPEC);
    int line_number = 0;
    while(!endSignal) {
        sleep(1);
        gdk_threads_enter();
        get_fstab_info(table_head);
        char *temp = refresh_content(line_number);
        line_number++;
        strcat(specific, temp);
        gtk_label_set_text(GTK_LABEL(label_output), specific);
        gdk_threads_leave();
    }
    return NULL;
}

void *thread_func3(void *arg) {
    int num = 0, sum = 0;
    while(!endSignal) {
        sleep(1);
        if (num > 1000)
            continue;
        gdk_threads_enter();
        gtk_label_set_text(GTK_LABEL(content), get_adder_info(&num, &sum));
        gdk_threads_leave();
    }
    return NULL;
}