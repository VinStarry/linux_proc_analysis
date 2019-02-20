//
// Created by vinstarry on 2/11/19.
//

#include "functions.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HIGHT 800
#define VALUE_MAX_LEN 100

GtkWidget *system_general_info(void) {
    GtkWidget *vbox = gtk_vbox_new(false, 0);

    GtkWidget *cpu_vbox = gtk_vbox_new(false, 0);
    GtkWidget *version_vbox = gtk_vbox_new(false, 0);
    GtkWidget *specific_vbox = gtk_vbox_new(false, 0);

    gtk_box_pack_start(GTK_BOX(vbox), cpu_vbox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), version_vbox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), specific_vbox, false, false, 0);
//    gtk_container_add(GTK_CONTAINER(vbox), specific_vbox);

    GtkWidget *label = gtk_label_new("CPU Information");
    gtk_label_set_markup(
            GTK_LABEL(label),
            "<span foreground='red' font_desc='24'>CPU Information</span>");
    gtk_container_add(GTK_CONTAINER(cpu_vbox), label);

    char *model_name = get_cpu_info_by_key("model name");
    if (model_name) {
        GtkWidget *cpu_model_name = gtk_label_new(model_name);
        gtk_container_add(GTK_CONTAINER(cpu_vbox), cpu_model_name);
    }

    char *cores = get_cpu_info_by_key("cpu cores");
    if (cores) {
        GtkWidget *cpu_cores = gtk_label_new(cores);
        gtk_container_add(GTK_CONTAINER(cpu_vbox), cpu_cores);
    }

    char *mhz = get_cpu_info_by_key("cpu MHz");
    if (mhz) {
        GtkWidget *cpu_mhz = gtk_label_new(mhz);
        gtk_container_add(GTK_CONTAINER(cpu_vbox), cpu_mhz);
    }

    char *c_size = get_cpu_info_by_key("cache size");
    if (c_size) {
        GtkWidget *cpu_cache_size = gtk_label_new(c_size);
        gtk_container_add(GTK_CONTAINER(cpu_vbox), cpu_cache_size);
    }

    GtkWidget *new_line = gtk_label_new("\n");
    gtk_container_add(GTK_CONTAINER(version_vbox), new_line);

    GtkWidget *version_label = gtk_label_new("Version Information");
    gtk_label_set_markup(
            GTK_LABEL(version_label),
            "<span foreground='red' font_desc='24'>Version Information</span>");

    gtk_container_add(GTK_CONTAINER(version_vbox), version_label);

    char *k_ver = get_kernel_version(0);
    GtkWidget *kernel_version = gtk_label_new(k_ver);
    gtk_container_add(GTK_CONTAINER(version_vbox), kernel_version);

    char *c_ver = get_kernel_version(1);
    GtkWidget *compiler_version = gtk_label_new(c_ver);
    gtk_container_add(GTK_CONTAINER(version_vbox), compiler_version);
    GtkWidget *new_line_2 = gtk_label_new("\n");
    gtk_container_add(GTK_CONTAINER(version_vbox), new_line_2);

    GtkWidget *specific_label = gtk_label_new("CPU Information");
    gtk_label_set_markup(
            GTK_LABEL(specific_label),
            "<span foreground='red' font_desc='24'>Specific Information</span>");
    gtk_container_add(GTK_CONTAINER(specific_vbox), specific_label);

    GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(specific_vbox),scrolled);
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

GtkWidget *get_proc_info(void) {
    mps *head,*link;

    head=trav_dir("/proc/");
    if(head == NULL)
        printf("traverse dir error\n");

    GtkWidget *rtn_vbox = print_ps(head);

    while(head!=NULL) {
        link=head;
        head=head->next;
        free(link);
    }

    return rtn_vbox;
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
        rtn[i++] = '\t';
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
        rtn[i++] = '\t';
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

int read_info(char *d_name, mps *p1)
{
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
    GtkWidget *vbox = gtk_vbox_new(false ,0);
    guint rows = 0;
    mps *cur;
    for (cur = head; cur != NULL; cur = cur->next) {
        rows++;
    }
    cur = head;
    rows--;
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(vbox),scrolled);
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

    return vbox;
}

unsigned int memory_info_lable_attach(lb_list *head) {
    lb_list *ptr = head;
    char name[VALUE_MAX_LEN] = {0};
    char value[VALUE_MAX_LEN] = {0};
    char ch;
    unsigned int lines = 0;
    FILE *fp = fopen("/proc/meminfo", "r");
    while ((ch = (char)fgetc(fp)) != EOF) {
        ptr->next = (lb_list *)malloc(sizeof(lb_list));
        ptr = ptr->next;
        ptr->next = NULL;
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
        ptr->lb1 = gtk_label_new(name);
        ptr->lb2 = gtk_label_new(value);
        lines++;
    }
    fclose(fp);
    return lines;
}

void attach_memory_info(GtkWidget *table, lb_list *head) {
    int i = 0;
    lb_list *ptr = head->next;
    GtkWidget *title_1 = gtk_label_new("Name");
    GtkWidget *title_2 = gtk_label_new("Value");
    gtk_table_attach(GTK_TABLE(table), title_1, 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), title_2, 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    i++;
    while (ptr) {
        gtk_table_attach(GTK_TABLE(table), ptr->lb1, 0, 1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        gtk_table_attach(GTK_TABLE(table), ptr->lb2, 1, 2, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        i++;
        ptr = ptr->next;
    }
}

void refresh_memory_info(lb_list *head) {
    lb_list *ptr = head->next;
    char name[VALUE_MAX_LEN] = {0};
    char value[VALUE_MAX_LEN] = {0};
    char ch;
    FILE *fp = fopen("/proc/meminfo", "r");
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
        gtk_label_set_text(GTK_LABEL(ptr->lb2), value);
        ptr = ptr->next;
    }
    fclose(fp);
}