//
// Created by vinstarry on 2/11/19.
//

#include "sys_page.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HIGHT 800
#define VALUE_MAX_LEN 100
#define SPECIFIC_LENGTH 1000

GtkWidget *system_general_info(void) {
    GtkWidget *vbox = gtk_vbox_new(false, 0);
    char sys_specific[SPECIFIC_LENGTH];
    char cm_brief[SPECIFIC_LENGTH];
    memset(sys_specific, 0, sizeof(sys_specific)/ sizeof(char));
    memset(cm_brief, 0, sizeof(cm_brief)/ sizeof(char));

    GtkWidget *sys_frame_title = gtk_label_new("");
    gtk_label_set_markup(
            GTK_LABEL(sys_frame_title),
            "<span foreground='brown' font_desc='24'>System information</span>");

    GtkWidget *cm_frame_title = gtk_label_new("");
    gtk_label_set_markup(
            GTK_LABEL(cm_frame_title),
            "<span foreground='brown' font_desc='24'>Brief information</span>");

    GtkWidget *cpu_frame_title = gtk_label_new("");
    gtk_label_set_markup(
            GTK_LABEL(cpu_frame_title),
            "<span foreground='brown' font_desc='24'>CPU specific</span>");

    GtkWidget *system_frame = gtk_frame_new("");
    gtk_frame_set_label_widget(GTK_FRAME(system_frame), sys_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(system_frame), 20);

    GtkWidget *cm_frame = gtk_frame_new("");
    gtk_frame_set_label_widget(GTK_FRAME(cm_frame), cm_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(cm_frame), 20);

    GtkWidget *cpu_specific = gtk_frame_new("");
    gtk_frame_set_label_widget(GTK_FRAME(cpu_specific), cpu_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(cpu_specific), 20);

    gtk_box_pack_start(GTK_BOX(vbox), system_frame, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), cm_frame, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), cpu_specific, false, false, 0);

    GtkWidget *label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(system_frame), label);

    sprintf(sys_specific, "%s\n%s\n%s\n%s\n",
            get_kernel_version(0),
            get_kernel_version(1),
            get_uptime(0),
            get_uptime(1));

    gtk_label_set_text(GTK_LABEL(label), sys_specific);

    GtkWidget *brief_label = gtk_label_new("");

    gtk_container_add(GTK_CONTAINER(cm_frame), brief_label);

    sprintf(cm_brief, "%s\n%s\n%s\n%s\n%s\n%s\n",
            get_hostname(),
            get_cpu_info_by_key("model name"),
            get_cpu_info_by_key("cpu cores"),
            get_cpu_info_by_key("cpu MHz"),
            get_cpu_info_by_key("cache size"),
            get_total_memory());

    gtk_label_set_text(GTK_LABEL(brief_label), cm_brief);

    GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(cpu_specific),scrolled);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

    GtkWidget *text=gtk_text_view_new();
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled),text);
    gtk_widget_set_size_request(scrolled, 0, 600);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
    char *specfics = get_specific_info();

    GtkTextIter start,end;

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer),&start,specfics,(gint)strlen(specfics));
    return vbox;
}

char *get_cpu_info_by_key(char *key) {
    char name[VALUE_MAX_LEN] = {0};
    char value[VALUE_MAX_LEN] = {0};
    char ch;
    FILE *fp = fopen("/proc/cpuinfo", "r");
    while ((ch = (char)fgetc(fp)) != EOF) {
        int i = 0, sp = 0, j = 0;
        while (ch != ':') {
            name[i++] = ch;
            ch = (char)fgetc(fp);
        }
        for (sp = i - 1; sp >= 0; sp--) {
            if(name[sp] != ' ' && name[sp] != '\t')
                break;
        }
        name[sp + 1] = '\0';
        fgetc(fp);
        ch = (char)fgetc(fp);
        while (ch != '\n') {
            value[j++] = ch;
            ch = (char)fgetc(fp);
        }
        value[j] = '\0';
        if (!strcmp(key, name)) {
            char *rtn = (char *)malloc(sizeof(char) * VALUE_MAX_LEN);
            strcpy(rtn, key);
            strcat(rtn, " :\t");
            strcat(rtn, value);
            return rtn;
        }
    }
    fclose(fp);
    return NULL;
}

char *get_kernel_version(int rank) {
    FILE *fp = fopen("/proc/version", "r");
    char *rtn = (char *)malloc(sizeof(char) * VALUE_MAX_LEN);
    char ch;
    int i = 0;
    if (rank == 0) {
        strcpy(rtn, "Kernel version:");
        for (i = 0; i < VALUE_MAX_LEN; i++)
            if (rtn[i] == '\0') {
                rtn[i] = ' ';
                break;
            }
        rtn[i++] = ' ';
        ch = (char)getc(fp);
        while (ch != EOF) {
            if (ch == '(') {
                rtn[i] = '\0';
                break;
            }
            rtn[i++] = ch;
            ch = (char)getc(fp);
        }
    }
    else {
        strcpy(rtn, "Compile version:");
        for (i = 0; i < VALUE_MAX_LEN; i++)
            if (rtn[i] == '\0') {
                rtn[i] = ' ';
                break;
            }
        rtn[i++] = ' ';
        ch = (char)getc(fp);
        int par_count = 0;
        while (ch != EOF) {
            if (ch == '(') {
                par_count++;
                ch = (char)getc(fp);
                continue;
            }
            if (par_count != 2) {
                ch = (char) getc(fp);
                continue;
            }
            if (ch == ')') {
                rtn[i] = '\0';
                break;
            }
            rtn[i++] = ch;
            ch = (char)getc(fp);
        }
    }
    fclose(fp);
    return rtn;
}

char *get_specific_info(void) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    long length = 0;
    char ch;
    while ((ch = (char)fgetc(fp)) != EOF) {
        length++;
    }
    length++;
    rewind(fp);
    char *rtn = (char *)malloc(sizeof(char) * length);
    int index = 0;
    while ((ch = (char)fgetc(fp)) != EOF) {
        rtn[index++] = ch;
    }
    rtn[index] = '\0';
    fclose(fp);
    return rtn;
}

char *get_uptime(int rank) {
    FILE *fp = fopen("/proc/uptime", "r");
    double up = 0, idle = 0;
    char *up_time = (char *)malloc(sizeof(char) * VALUE_MAX_LEN);
    char *idle_time = (char *)malloc(sizeof(char) * VALUE_MAX_LEN);

    if (fp == NULL) {
        strcpy(up_time, "Open /proc/uptime error");
        strcpy(idle_time, "Open /proc/uptime error");
    }
    else {
        fscanf(fp, "%lf %lf", &up, &idle);

        if (up <= 60) {
            sprintf(up_time, "Up time: %.2lf seconds", up);
        }
        else if (up <= 60 * 60) {
            sprintf(up_time, "Up time: %.2lf minutes", up / 60);
        }
        else{
            sprintf(up_time, "Up time: %.2lf days", up / 60 / 60);
        }

        if (idle <= 60) {
            sprintf(idle_time, "Idle time: %.2lf seconds", idle);
        }
        else if (idle <= 60 * 60) {
            sprintf(idle_time, "Idle time: %.2lf minutes", idle / 60);
        }
        else{
            sprintf(idle_time, "Idle time: %.2lf days", idle / 60 / 60);
        }

    }

    fclose(fp);
    if (rank == 0) {
        free(idle_time);
        idle_time = NULL;
        return up_time;
    }
    else {
        free(up_time);
        up_time = NULL;
        return idle_time;
    }
}

char *get_total_memory(void) {
    FILE *fp = fopen("/proc/meminfo", "r");
    double up = 0, idle = 0;
    char *rtn = (char *)malloc(sizeof(char) * VALUE_MAX_LEN);
    char memtotal[VALUE_MAX_LEN / 2];
    unsigned long sz;

    if (fp == NULL) {
        strcpy(rtn, "Open /proc/meminfo error");
    }
    else {
        fscanf(fp, "%s %lu", memtotal, &sz);
        if (sz <= 1024) {
            sprintf(rtn, "%s\t%lu KB", memtotal, sz);
        }
        else if (sz <= 1024 * 1024) {
            sprintf(rtn, "%s\t%lu MB", memtotal, (unsigned long)(sz / 1024.0 + 0.5));
        }
        else {
            sprintf(rtn, "%s\t%lu GB", memtotal, (unsigned long)(sz / 1024.0 / 1024.0 + 0.5));
        }
    }
    fclose(fp);

    return rtn;
}

char *get_hostname(void) {
    FILE *fp = fopen("/proc/sys/kernel/hostname", "r");
    double up = 0, idle = 0;
    char *rtn = (char *)malloc(sizeof(char) * VALUE_MAX_LEN);
    char hostname[VALUE_MAX_LEN / 2];

    if (fp == NULL) {
        strcpy(rtn, "Open /proc/sys/kernel/hostname error");
    }
    else {
        fscanf(fp, "%s", hostname);
        if (hostname[0] < 'z' && hostname[0] > 'a')
            hostname[0] += ('A' - 'a');
        sprintf(rtn, "Hostname:\t%s", hostname);
    }

    fclose(fp);

    return rtn;
}