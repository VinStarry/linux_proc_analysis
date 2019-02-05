//
// Created by vinstarry on 1/15/19.
//

#ifndef GTKFIRST_WINDOW_FUNCTIONS_H
#define GTKFIRST_WINDOW_FUNCTIONS_H
#define BUF_LEN 50

#include <unistd.h>
#include <time.h>
#include <malloc.h>

char *get_system_time(void);
char *get_adder_info(int *number, int *sum);
char *get_cpu_usager(void);

#endif //GTKFIRST_WINDOW_FUNCTIONS_H
