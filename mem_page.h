//
// Created by vinstarry on 2/20/19.
//

#ifndef PROC_ANALYSIS_MEM_PAGE_H
#define PROC_ANALYSIS_MEM_PAGE_H

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

typedef struct _label_list{
    GtkWidget *lb1;
    GtkWidget *lb2;
    struct _label_list *next;
}lb_list;

void refresh_memory_info(lb_list *head);
unsigned int memory_info_lable_attach(lb_list *head);
void attach_memory_info(GtkWidget *table, lb_list *head);

#endif //PROC_ANALYSIS_MEM_PAGE_H
