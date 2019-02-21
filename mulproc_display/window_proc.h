//
// Created by vinstarry on 2/8/19.
//

#ifndef MULPROC_DISPLAY_WINDOW_PROC_H
#define MULPROC_DISPLAY_WINDOW_PROC_H

#include <gtk/gtk.h>
#include <malloc.h>
#include <stdbool.h>
#include "table_op.h"

/* use argv and argc and rank (0,1,2) to create window proc */
void wind_proc(int argc, char *argv[], int rank);
/* destroy singal handle */
gboolean destroy_handle(GtkWidget *self, GdkEvent *event, gpointer data);
/* get title name from rank */
char *title_name(int rank);
/* start button pressed, call back start_display */
void start_display(GtkWidget *self, GdkEvent *event, gpointer data);
/* stop button pressed, call back stop_display */
void stop_display(GtkWidget *self, GdkEvent *event, gpointer data);
/* get the system time */
char *get_system_time(void);
/* get adder info */
char *get_adder_info(int *number, int *sum);
/* get cpu usage by all time and idle time */
char *get_cpu_usager(long all1, long all2, long idle1, long idle2);
/* refresh fstab content */
char *refresh_content(int line_no);
/* thread1 function */
void *thread_func1(void *arg);
/* thread2 function */
void *thread_func2(void *arg);
/* thread3 function */
void *thread_func3(void *arg);

#endif //MULPROC_DISPLAY_WINDOW_PROC_H
