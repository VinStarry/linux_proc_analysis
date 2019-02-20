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

GtkWidget *system_general_info(void);
char *get_cpu_info_by_key(char *key);
char *get_kernel_version(int rank);
char *get_specific_info(void);
char *get_uptime(int rank);
char *get_total_memory(void);
char *get_hostname(void);

#endif //PROC_ANALYSIS_FUNCTIONS_H
