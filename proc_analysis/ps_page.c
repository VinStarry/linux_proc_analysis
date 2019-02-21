//
// Created by vinstarry on 2/20/19.
//

#include "ps_page.h"

#define BUF_LEN 50
#define LINE_CHART_REAL_WIDTH 1200
#define LINE_CHART_REAL_HEIGHT 160
#define LINE_CHART_HORIZONTAL_BEGIN_POS 120
#define LINE_CHART_LINE_SPACING 20
#define LINE_CHART_DOT_STEP 120
#define ARRAY_SIZE 10

double cpu_usage_percent_array[ARRAY_SIZE] = {0.0};
double mem_usage_percent_array[ARRAY_SIZE] = {0.0};

GtkWidget *get_proc_info(void) {
    mps *head,*link;

    head=trav_dir("/proc/");
    if(head == NULL)
        printf("traverse dir error\n");

    GtkWidget *rtn_frame = print_ps(head);

    while(head!=NULL) {
        link=head;
        head=head->next;
        free(link);
    }

    return rtn_frame;
}

mps *trav_dir(char *dir) {
    DIR *dir_ptr;
    mps *head = NULL, *p1 = NULL;
    struct dirent *direntp;

    if (!(dir_ptr=opendir(dir)))
        fprintf(stderr,"dir error %s\n",dir);
    else {
        head = p1 = (mps *)malloc(sizeof(mps));
        while((direntp=readdir(dir_ptr)) != NULL) {
            if ((str_pure_num(direntp->d_name)) == true) {
                if (!p1) {
                    printf("malloc error!\n");
                    exit(0);
                }
                if(read_info(direntp->d_name,p1)!=0) {
                    printf("read_info error\n");
                    exit(0);
                }
                p1->next = (struct ps_info *)malloc(sizeof(struct ps_info));
                p1 = p1->next;
                p1->next = NULL;
            }
        }
    }
    return head;
}

int read_info(char *d_name, mps *p1) {
    FILE *fd;
    char dir[20];
    struct stat infobuf;

    sprintf(dir,"%s/%s","/proc/",d_name);
    chdir("/proc");                        //切换至/proc目录，不然stat返回-1
    if (stat(d_name,&infobuf) !=-1)                     //get process USER
        uid_to_name(infobuf.st_uid, p1);
    chdir(dir);                         //切换到/proc/pid目录
    if ((fd = fopen("stat","r")) < 0) {
        printf("open the file is error!\n");
        exit(0);
    }

    fscanf(fd,"%d %s %c %d %d %d %d %d %lu %d %d %d %d %d %d %d %d %d %d %d %d %d %lu %lu\n", &(p1->pid), p1->pname, &(p1->state), &(p1->ppid),
           &(p1->pgid), &(p1->sid), &(p1->tty_nr), &(p1->tty_pgrp), &(p1->task_flags), &(p1->min_flt),
           &(p1->cmin_flt), &(p1->maj_flt), &(p1->cmaj_flt), &(p1->utime), &(p1->stime), &(p1->cutime), &(p1->cstime), &(p1->priority),
           &(p1->nice), &(p1->num_threads), &(p1->it_real_value), &(p1->start_time), &(p1->vsize), &(p1->rss));
    fclose(fd);

    if ((fd = fopen("cmdline","r")) < 0) {
        strcpy(p1->cmdline, "none");
    }
    else {
        char ch;
        int i = 0;
        while ( (ch = (char)fgetc(fd)) != EOF) {
            if (i == 98) {
                break;
            }
            p1->cmdline[i++] = ch;
        }
        p1->cmdline[i] = '\0';
    }
    fclose(fd);
    return 0;
}

bool str_pure_num(char *str) {
    bool result = true;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            result = false;
            break;
        }
    }
    return result;
}

void uid_to_name(uid_t uid, struct ps_info *p1) {
    struct passwd *pw_ptr;
    static char numstr[10];

    if ((pw_ptr=getpwuid(uid)) == NULL) {
        sprintf(numstr,"%d",uid);
        strcpy(p1->user, numstr);
    }
    else
        strcpy(p1->user, pw_ptr->pw_name);
}

GtkWidget* print_ps(struct ps_info *head) {
    GtkWidget *ps_frame_title = gtk_label_new("");
    gtk_label_set_markup(
            GTK_LABEL(ps_frame_title),
            "<span foreground='brown' font_desc='24'>Process information</span>");
    GtkWidget *ps_frame = gtk_frame_new("");
    gtk_frame_set_label_widget(GTK_FRAME(ps_frame), ps_frame_title);
    gtk_container_set_border_width(GTK_CONTAINER(ps_frame), 20);

    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    cur = head;
    rows--;
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(ps_frame),scrolled);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    GtkWidget* pHtable = gtk_table_new(rows, 7, false);
    gtk_scrolled_window_add_with_viewport (
            GTK_SCROLLED_WINDOW (scrolled), pHtable);
    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char pname[20];
    char state[10];
    GtkWidget *usr_lb[rows], *pid_lb[rows], *vsz_lb[rows], *rss_lb[rows], *state_lb[rows], *cmd_lb[rows], *pn_lb[rows];
    for (int i = 0; i < rows; i++) {
        if (i == 0) {
            strcpy(usr, "USER");
            strcpy(pid, "PID");
            strcpy(vsz, "VSZ");
            strcpy(rss, "RSS");
            strcpy(command, "COMMAND");
            strcpy(pname, "PNAME");
            strcpy(state, "STATE");
            usr_lb[i] = gtk_label_new(usr);
            pid_lb[i] = gtk_label_new(pid);
            vsz_lb[i] = gtk_label_new(vsz);
            rss_lb[i] = gtk_label_new(rss);
            state_lb[i] = gtk_label_new(state);
            cmd_lb[i] = gtk_label_new(command);
            pn_lb[i] = gtk_label_new(pname);
            gtk_table_attach(GTK_TABLE(pHtable), usr_lb[i], 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), pid_lb[i], 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), vsz_lb[i], 2, 3, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), rss_lb[i], 3, 4, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), state_lb[i], 4, 5, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), cmd_lb[i], 5, 6, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), pn_lb[i], 6, 7, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        }
        else {
            if (i != 1)
                cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(pname, "%s", cur->pname);
            sprintf(state, "%c", cur->state);
            usr_lb[i] = gtk_label_new(usr);
            pid_lb[i] = gtk_label_new(pid);
            vsz_lb[i] = gtk_label_new(vsz);
            rss_lb[i] = gtk_label_new(rss);
            state_lb[i] = gtk_label_new(state);
            cmd_lb[i] = gtk_label_new(command);
            pn_lb[i] = gtk_label_new(pname);
            gtk_table_attach(GTK_TABLE(pHtable), usr_lb[i], 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), pid_lb[i], 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), vsz_lb[i], 2, 3, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), rss_lb[i], 3, 4, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), state_lb[i], 4, 5, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), cmd_lb[i], 5, 6, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), pn_lb[i], 6, 7, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        }
    }

    return ps_frame;
}

gboolean update_mem_usage (gpointer user_data) {
    char *usage_text = (char *)malloc(sizeof(char) * BUF_LEN);

    FILE *fp = fopen("/proc/meminfo", "r");
    char total_name[BUF_LEN];
    char free_name[BUF_LEN];
    char kb[BUF_LEN];
    unsigned long total_volume, free_volum;

    fscanf(fp, "%s %ld %s", total_name, &total_volume, kb);
    fscanf(fp, "%s %ld %s", free_name, &free_volum, kb);

//    printf("%ld, %ld\n", total_volume, free_volum);

    fclose(fp);

    double usage = (double)(total_volume - free_volum) * 100.0 / (double)(total_volume);
    sprintf(usage_text, "Memory usage = %.2f%%(%ldKB/ %ldKB)\n", usage, free_volum, total_volume);

    add_mem_usage_into_array(usage);

    GtkLabel *label = GTK_LABEL(user_data);

    gtk_label_set_label(label, usage_text);

    free(usage_text);
    usage_text = NULL;

    return G_SOURCE_CONTINUE; /* or G_SOURCE_REMOVE when you want to stop */
}

gboolean update_cpu_usage (gpointer user_data) {
    static long all1 = 0, idle1 = 0, all2 = 0, idle2 = 0;

    char *usage_text = (char *)malloc(sizeof(char) * BUF_LEN);

    if (all1 == 0 && idle1 == 0) {
        // initial
        char buf[128] = {'\0'};
        char cpu[5];
        long int user, nice, sys, idle, iowait, irq, softirq;

        FILE *fp = fopen("/proc/stat", "r");
        fgets(buf, sizeof(buf), fp);
        sscanf(buf, "%s%ld%ld%ld%ld%ld%ld%ld", cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);

        all1 = user + nice + sys + idle + iowait + irq + softirq;
        idle1 = idle;
        fclose(fp);

        sprintf(usage_text, "cpu usage = %.2f%%\n", 0.0);
    }
    else {
        // data has gained
        float usage;

        char buf[128] = {'\0'};
        char cpu[5];
        long int user, nice, sys, idle, iowait, irq, softirq;

        FILE *fp = fopen("/proc/stat", "r");
        fgets(buf, sizeof(buf), fp);
        sscanf(buf, "%s%ld%ld%ld%ld%ld%ld%ld", cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);

        all2 = user + nice + sys + idle + iowait + irq + softirq;
        idle2 = idle;
        fclose(fp);

        usage = (float) (all2 - all1 - (idle2 - idle1)) / (all2 - all1) * 100;

        sprintf(usage_text, "cpu usage = %.2f%%\n", usage);
        idle1 = idle2;
        all1 = all2;
        add_cpu_usage_into_array(usage);
    }

    GtkLabel *label = GTK_LABEL(user_data);

    gtk_label_set_label(label, usage_text);

    free(usage_text);
    usage_text = NULL;

    return G_SOURCE_CONTINUE; /* or G_SOURCE_REMOVE when you want to stop */
}

gboolean usage_cpu_draw(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    gtk_timeout_add(1000, (GtkFunction)fresh_cpu_record, (gpointer)widget);
    return TRUE;
}

gboolean usage_mem_draw(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    gtk_timeout_add(1000, (GtkFunction)fresh_mem_record, (gpointer)widget);
    return TRUE;
}

gboolean fresh_cpu_record(GtkWidget *widget) {
    GdkColor color;
    GdkDrawable *canvas;
    GdkGC *gc;

    canvas = widget->window;
    gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];

    color.red = 0;
    color.green = 0;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);

    gdk_draw_rectangle(canvas, gc, TRUE, LINE_CHART_HORIZONTAL_BEGIN_POS, 0, LINE_CHART_REAL_WIDTH, LINE_CHART_REAL_HEIGHT);
    color.red = 0;
    color.green = 30000;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);
    for (int i = LINE_CHART_LINE_SPACING; i < LINE_CHART_REAL_HEIGHT; i += LINE_CHART_LINE_SPACING) {
        // draw horizontal lines
        gdk_draw_line(canvas, gc, LINE_CHART_HORIZONTAL_BEGIN_POS, i, LINE_CHART_REAL_WIDTH + LINE_CHART_HORIZONTAL_BEGIN_POS, i);
    }
    
    color.red = 0;
    color.green = 65535;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);

    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        if (cpu_usage_percent_array[i] == 0 || cpu_usage_percent_array[i + 1] == 0) {
            break;
        }
        else {
            gdk_draw_line(canvas, gc,
                    LINE_CHART_HORIZONTAL_BEGIN_POS + LINE_CHART_DOT_STEP * i, (int)((1 - cpu_usage_percent_array[i] / 100) * LINE_CHART_REAL_HEIGHT),
                          LINE_CHART_HORIZONTAL_BEGIN_POS + LINE_CHART_DOT_STEP * (i + 1), (int)((1 - cpu_usage_percent_array[i + 1] / 100) * LINE_CHART_REAL_HEIGHT)
                    );
        }
    }

    color.red = 0;
    color.green = 0;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);

    return G_SOURCE_CONTINUE;
}

gboolean fresh_mem_record(GtkWidget *widget) {
    GdkColor color;
    GdkDrawable *canvas;
    GdkGC *gc;

    canvas = widget->window;
    gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];

    color.red = 0;
    color.green = 0;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);

    gdk_draw_rectangle(canvas, gc, TRUE, LINE_CHART_HORIZONTAL_BEGIN_POS, 0, LINE_CHART_REAL_WIDTH, LINE_CHART_REAL_HEIGHT);
    color.red = 0;
    color.green = 30000;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);
    for (int i = LINE_CHART_LINE_SPACING; i < LINE_CHART_REAL_HEIGHT; i += LINE_CHART_LINE_SPACING) {
        // draw horizontal lines
        gdk_draw_line(canvas, gc, LINE_CHART_HORIZONTAL_BEGIN_POS, i, LINE_CHART_REAL_WIDTH + LINE_CHART_HORIZONTAL_BEGIN_POS, i);
    }

    color.red = 0;
    color.green = 65535;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);

    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        if (mem_usage_percent_array[i] == 0 || mem_usage_percent_array[i + 1] == 0) {
            break;
        }
        else {
            gdk_draw_line(canvas, gc,
                          LINE_CHART_HORIZONTAL_BEGIN_POS + LINE_CHART_DOT_STEP * i, (int)((1 - mem_usage_percent_array[i] / 100) * LINE_CHART_REAL_HEIGHT),
                          LINE_CHART_HORIZONTAL_BEGIN_POS + LINE_CHART_DOT_STEP * (i + 1), (int)((1 - mem_usage_percent_array[i + 1] / 100) * LINE_CHART_REAL_HEIGHT)
            );
        }
    }

    color.red = 0;
    color.green = 0;
    color.blue = 0;
    gdk_gc_set_rgb_fg_color(gc, &color);

    return G_SOURCE_CONTINUE;
}

void add_cpu_usage_into_array(double usage) {
    bool all_none_zero = true;
    for (int i = 0; i < ARRAY_SIZE ; i++) {
        if (cpu_usage_percent_array[i] == 0.0) {
            cpu_usage_percent_array[i] = usage;
            all_none_zero = false;
            break;
        }
    }
    if (all_none_zero) {
        for (int i = 0; i < ARRAY_SIZE - 1; i++)
            cpu_usage_percent_array[i] = cpu_usage_percent_array[i + 1];
        cpu_usage_percent_array[ARRAY_SIZE - 1] = usage;
    }
}

void add_mem_usage_into_array(double usage) {
    bool all_none_zero = true;
    for (int i = 0; i < ARRAY_SIZE ; i++) {
        if (mem_usage_percent_array[i] == 0.0) {
            mem_usage_percent_array[i] = usage;
            all_none_zero = false;
            break;
        }
    }
    if (all_none_zero) {
        for (int i = 0; i < ARRAY_SIZE - 1; i++)
            mem_usage_percent_array[i] = mem_usage_percent_array[i + 1];
        mem_usage_percent_array[ARRAY_SIZE - 1] = usage;
    }
}

void debug_print_cpu_array(void) {
    for (int i = 0; i < ARRAY_SIZE ; i++) {
        if (cpu_usage_percent_array[i] == 0.0)
            break;
        printf("%.2lf ", cpu_usage_percent_array[i]);
    }
    putchar('\n');
}

void init_cpu_array(void) {
    for (int i = 0; i < ARRAY_SIZE ; i++) {
        cpu_usage_percent_array[i] = 0.0;
        mem_usage_percent_array[i] = 0.0;
    }
}