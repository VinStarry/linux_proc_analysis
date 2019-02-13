//
// Created by vinstarry on 2/11/19.
//

#ifndef PROC_ANALYSIS_FUNCTIONS_H
#define PROC_ANALYSIS_FUNCTIONS_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

typedef struct ps_info
{
    char pname[20];
    char user[20];
    int  pid;
    int  ppid;
    int pgid;
    int sid;
    int tty_nr;
    int tty_pgrp;
    int min_flt;
    int cmin_flt;
    int maj_flt;
    int cmaj_flt;
    int utime;
    int stime;
    int cutime;
    int cstime;
    int priority;
    int nice;
    int num_threads;
    int it_real_value;
    int start_time;
    unsigned long vsize;
    unsigned long rss;
    unsigned long task_flags;
    char state;
    char cmdline[100];
    struct ps_info *next;
}mps;

typedef struct _label_list{
    GtkWidget *lb1;
    GtkWidget *lb2;
    struct _label_list *next;
}lb_list;

GtkWidget *system_general_info(void);
GtkWidget *get_proc_info(void);
char *get_cpu_info_by_key(char *key);
char *get_kernel_version(int rank);
char *get_specific_info(void);
mps *trav_dir(char *dir);
int read_info(char *d_name, struct ps_info *p1);
GtkWidget* print_ps(struct ps_info *head);
bool str_pure_num(char *str);
void uid_to_name(uid_t uid, struct ps_info *p1);         //由进程uid得到进程的所有者user
unsigned int memory_info_lable_attach(lb_list *head);
void attach_memory_info(GtkWidget *table, lb_list *head);
void refresh_memory_info(lb_list *head);

#endif //PROC_ANALYSIS_FUNCTIONS_H
