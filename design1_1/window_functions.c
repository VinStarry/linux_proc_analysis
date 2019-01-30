//
// Created by vinstarry on 1/15/19.
//

#include "window_functions.h"

char *get_system_time(void) {
    char *time_text = (char *)malloc(sizeof(char) * BUF_LEN);
    time_t local_time;
    struct tm *lt;
    time(&local_time);
    lt = localtime (&local_time);
    int len = sprintf(time_text, "%d/%d/%d %d:%d:%d\n", lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    time_text[len] = '\0';
    return time_text;
}