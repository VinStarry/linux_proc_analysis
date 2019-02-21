//
// Created by vinstarry on 2/20/19.
//

#ifndef PROC_ANALYSIS_PS_PAGE_H
#define PROC_ANALYSIS_PS_PAGE_H

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

typedef struct ps_info {
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

GtkWidget *get_proc_info(void);
mps *trav_dir(char *dir);
int read_info(char *d_name, struct ps_info *p1);
GtkWidget* print_ps(struct ps_info *head);
bool str_pure_num(char *str);
void uid_to_name(uid_t uid, struct ps_info *p1);         //由进程uid得到进程的所有者user
gboolean update_cpu_usage (gpointer user_data);
gboolean update_mem_usage (gpointer user_data);
gboolean usage_cpu_draw(GtkWidget *widget,GdkEventExpose *event,gpointer data);
gboolean usage_mem_draw(GtkWidget *widget,GdkEventExpose *event,gpointer data);
gboolean fresh_cpu_record(GtkWidget *widget);
gboolean fresh_mem_record(GtkWidget *widget);
void add_cpu_usage_into_array(double usage);
void add_mem_usage_into_array(double usage);
void debug_print_cpu_array(void);
void init_cpu_array(void);

#endif //PROC_ANALYSIS_PS_PAGE_H
