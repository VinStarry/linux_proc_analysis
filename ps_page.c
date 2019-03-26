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
#define ROW_MAX 400

double cpu_usage_percent_array[ARRAY_SIZE] = {0.0};
double mem_usage_percent_array[ARRAY_SIZE] = {0.0};
long page_size = 0;
long memory_total_size = 0;
long sys_frequency = 0;
int sort_num = 0;

mps *head = NULL;
GtkWidget *usr_lb[ROW_MAX + 1], *pid_lb[ROW_MAX + 1], *vsz_lb[ROW_MAX + 1], *rss_lb[ROW_MAX + 1], *state_lb[ROW_MAX + 1], *cmd_lb[ROW_MAX + 1], *mem_per_lb[ROW_MAX + 1], *cpu_per_lb[ROW_MAX + 1];

void get_system_config(void) {
    page_size = sysconf(_SC_PAGE_SIZE);
    sys_frequency = sysconf(_SC_CLK_TCK);

    FILE *fp = fopen("/proc/meminfo", "r");
    char total_name[BUF_LEN];

    fscanf(fp, "%s %ld", total_name, &memory_total_size);

    fclose(fp);
}

GtkWidget *get_proc_info(void) {
    get_system_config();

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
    mps *p1 = NULL;
    struct dirent *direntp;

    if (!(dir_ptr=opendir(dir)))
        fprintf(stderr,"dir error %s\n",dir);
    else {
        head = p1 = (mps *)malloc(sizeof(mps));
        head->next = (mps *)malloc(sizeof(mps));
        p1 = head->next;
        p1->next = NULL;
        while((direntp = readdir(dir_ptr)) != NULL) {
            if ((str_pure_num(direntp->d_name)) == true) {

                if(read_info(direntp->d_name,p1) != 0) {
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
    chdir("/proc");

    if (stat(d_name,&infobuf) !=-1)
        uid_to_name(infobuf.st_uid, p1);
    chdir(dir);

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

    long up_time = get_up_time();

    double seconds = (up_time * 1.0) - ((p1->start_time * 1.0) / (sys_frequency *1.0));
    double total_tm = (p1->stime + p1->utime) * 100.0 / (sys_frequency * 1.0);

    p1->mem_usage = ((double)(p1->rss) * (double)(page_size) / 1024.0 / (double)memory_total_size) * 100.0;
    p1->cpu_usage = total_tm / (seconds * 1.0);

    p1->rss = p1->rss * page_size / 1024;

    fclose(fd);
    return 0;
}

void init_proc_table(GtkWidget *table) {
    get_system_config();

    head = trav_dir("/proc/");
    if(head == NULL)
        printf("traverse dir error\n");

    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    cur = head;
    rows--;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    for (int i = 0; i < ROW_MAX; i++) {
        if (i == 0) {
            continue;
        }
        else {
            if (i < rows) {
                cur = cur->next;
                sprintf(usr, "%s", cur->user);
                sprintf(pid, "%d", cur->pid);
                sprintf(vsz, "%lu", cur->vsize);
                sprintf(rss, "%lu", cur->rss);
                sprintf(command, "%s", cur->cmdline);
                sprintf(mem_per, "%.2lf", cur->mem_usage);
                sprintf(cpu_per, "%.2lf", cur->cpu_usage);
                sprintf(state, "%c", cur->state);
            }
            else {
                strcpy(usr, " ");
                strcpy(pid, " ");
                strcpy(vsz, " ");
                strcpy(rss, " ");
                strcpy(command, " ");
                strcpy(mem_per, " ");
                strcpy(cpu_per, " ");
                strcpy(state, " ");
            }
            usr_lb[i] = gtk_label_new(usr);
            pid_lb[i] = gtk_label_new(pid);
            vsz_lb[i] = gtk_label_new(vsz);
            rss_lb[i] = gtk_label_new(rss);
            state_lb[i] = gtk_label_new(state);
            cmd_lb[i] = gtk_label_new(command);
            mem_per_lb[i] = gtk_label_new(mem_per);
            cpu_per_lb[i] = gtk_label_new(cpu_per);
            gtk_table_attach(GTK_TABLE(table), pid_lb[i], 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), usr_lb[i], 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), mem_per_lb[i], 2, 3, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), cpu_per_lb[i], 3, 4, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), vsz_lb[i], 4, 5, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), rss_lb[i], 5, 6, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), state_lb[i], 6, 7, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(table), cmd_lb[i], 7, 8, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        }
    }
}

void free_mps(void) {
    mps *prev;

    while(head != NULL) {
        prev = head;
        head = head->next;
        free(prev);
    }

    head = NULL;
}

gboolean update_proc_table(gpointer table) {
    get_system_config();

    free_mps();

    head=trav_dir("/proc/");
    if(head == NULL)
        printf("traverse dir error\n");

    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    cur = head;
    rows--;

    struct_sort(sort_num, rows);

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }

    return G_SOURCE_CONTINUE; /* or G_SOURCE_REMOVE when you want to stop */
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
    char mem_per[20];
    char state[10];
    GtkWidget *usr_lb[rows], *pid_lb[rows], *vsz_lb[rows], *rss_lb[rows], *state_lb[rows], *cmd_lb[rows], *mem_per_lb[rows];
    for (int i = 0; i < rows; i++) {
        if (i == 0) {
            strcpy(usr, "USER");
            strcpy(pid, "PID");
            strcpy(mem_per, "%MEM");
            strcpy(vsz, "VSZ");
            strcpy(rss, "RSS");
            strcpy(command, "COMMAND");
            strcpy(state, "STATE");
            usr_lb[i] = gtk_label_new(usr);
            pid_lb[i] = gtk_label_new(pid);
            vsz_lb[i] = gtk_label_new(vsz);
            rss_lb[i] = gtk_label_new(rss);
            state_lb[i] = gtk_label_new(state);
            cmd_lb[i] = gtk_label_new(command);
            mem_per_lb[i] = gtk_label_new(mem_per);
            gtk_table_attach(GTK_TABLE(pHtable), usr_lb[i], 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), pid_lb[i], 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), mem_per_lb[i], 2, 3, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), vsz_lb[i], 3, 4, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), rss_lb[i], 4, 5, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), state_lb[i], 5, 6, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), cmd_lb[i], 6, 7, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        }
        else {
            if (i != 1)
                cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(state, "%c", cur->state);
            usr_lb[i] = gtk_label_new(usr);
            pid_lb[i] = gtk_label_new(pid);
            vsz_lb[i] = gtk_label_new(vsz);
            rss_lb[i] = gtk_label_new(rss);
            state_lb[i] = gtk_label_new(state);
            cmd_lb[i] = gtk_label_new(command);
            mem_per_lb[i] = gtk_label_new(mem_per);
            gtk_table_attach(GTK_TABLE(pHtable), usr_lb[i], 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), pid_lb[i], 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), mem_per_lb[i], 2, 3, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), vsz_lb[i], 3, 4, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), rss_lb[i], 4, 5, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), state_lb[i], 5, 6, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_table_attach(GTK_TABLE(pHtable), cmd_lb[i], 6, 7, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        }
    }

    return ps_frame;
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

gboolean usage_cpu_draw(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    gtk_timeout_add(1000, (GtkFunction)fresh_cpu_record, (gpointer)widget);
    return TRUE;
}

gboolean sort_by_pid(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    rows--;

    cur = head;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    sort_num = 0;
    struct_sort(sort_num, rows);

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }
    return true;
}

gboolean sort_by_user(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    rows--;

    cur = head;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    sort_num = 1;
    struct_sort(sort_num, rows);

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }
    return true;
}

gboolean sort_by_memper(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    rows--;

    cur = head;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    sort_num = 2;
    struct_sort(sort_num, rows);

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }
    return true;
}

gboolean sort_by_cpuper(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    rows--;

    cur = head;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    sort_num = 3;
    struct_sort(sort_num, rows);

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }
    return true;
}

gboolean sort_by_rss(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    rows--;

    cur = head;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    sort_num = 5;
    struct_sort(sort_num, rows);

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }
    return true;
}

gboolean sort_by_vsz(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    rows--;

    cur = head;

    char usr[20];
    char pid[20];
    char vsz[20];
    char rss[20];
    char command[100];
    char mem_per[20];
    char cpu_per[20];
    char state[10];

    sort_num = 4;
    struct_sort(sort_num, rows);

    for (int i = 1; i < ROW_MAX; i++) {
        if (i < rows) {
            cur = cur->next;
            sprintf(usr, "%s", cur->user);
            sprintf(pid, "%d", cur->pid);
            sprintf(vsz, "%lu", cur->vsize);
            sprintf(rss, "%lu", cur->rss);
            sprintf(command, "%s", cur->cmdline);
            sprintf(mem_per, "%.2lf", cur->mem_usage);
            sprintf(cpu_per, "%.2lf", cur->cpu_usage);
            sprintf(state, "%c", cur->state);
            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
        else {
            strcpy(usr, " ");
            strcpy(pid, " ");
            strcpy(vsz, " ");
            strcpy(rss, " ");
            strcpy(command, " ");
            strcpy(mem_per, " ");
            strcpy(cpu_per, " ");
            strcpy(state, " ");

            gtk_label_set_text(GTK_LABEL(pid_lb[i]), pid);
            gtk_label_set_text(GTK_LABEL(usr_lb[i]), usr);
            gtk_label_set_text(GTK_LABEL(mem_per_lb[i]), mem_per);
            gtk_label_set_text(GTK_LABEL(cpu_per_lb[i]), cpu_per);
            gtk_label_set_text(GTK_LABEL(vsz_lb[i]), vsz);
            gtk_label_set_text(GTK_LABEL(rss_lb[i]), rss);
            gtk_label_set_text(GTK_LABEL(state_lb[i]), state);
            gtk_label_set_text(GTK_LABEL(cmd_lb[i]), command);
        }
    }
    return true;
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

long get_up_time(void) {
    FILE *fp = fopen("/proc/uptime", "r");
    long up_time = 0;

    fscanf(fp, "%ld", &up_time);

    fclose(fp);

    return up_time;
}

void struct_sort(int col_num, int rows) {
    mps *temp_head = head;
    for (int i = 0; i < rows; i++) {
        mps *base = temp_head->next;
        bool change = false;
        mps *min = base;
        for (int j = i; j < rows - 1; j++) {
            bool smaller = false;
            switch (col_num) {
                case 0:
                    smaller = (base->pid < min->pid);
                    break;
                case 1:
                    smaller = (strcmp(base->user, min->user) < 0);
                    break;
                case 2:
                    smaller = (base->mem_usage > min->mem_usage);
                    break;
                case 3:
                    smaller = (base->cpu_usage > min->cpu_usage);
                    break;
                case 4:
                    smaller = (base->vsize > min->vsize);
                    break;
                case 5:
                    smaller = (base->rss > min->rss);
                    break;
                default:
                    break;
            }
            if (smaller) {
                min = base;
                change = true;
            }
            base = base->next;
        }
        if (change) {
            mps *temp_prev = head;
            while(temp_prev->next != min)
                temp_prev = temp_prev->next;
            temp_prev->next = min->next;
            min->next = temp_head->next;
            temp_head->next = min;
        }
        temp_head = temp_head->next;
    }
}