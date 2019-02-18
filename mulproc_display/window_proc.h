//
// Created by vinstarry on 2/8/19.
//

#ifndef MULPROC_DISPLAY_WINDOW_PROC_H
#define MULPROC_DISPLAY_WINDOW_PROC_H

#include <gtk/gtk.h>
#include <malloc.h>
#include <stdbool.h>
#include "table_op.h"

void wind_proc(int argc, char *argv[], int rank);
gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);
char *title_name(int rank);
void start_display(GtkWidget *self, GdkEvent *event, gpointer data);
void stop_display(GtkWidget *self, GdkEvent *event, gpointer data);
char *get_system_time(void);
char *get_adder_info(int *number, int *sum);
char *get_cpu_usager(long all1, long all2, long idle1, long idle2);
void *thread_func1(void *arg);
void *thread_func2(void *arg);
void *thread_func3(void *arg);

#endif //MULPROC_DISPLAY_WINDOW_PROC_H
