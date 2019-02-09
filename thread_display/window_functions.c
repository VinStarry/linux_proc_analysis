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

char *get_adder_info(int *number, int *sum) {
    char *adder_text = (char *)malloc(sizeof(char) * BUF_LEN);
    sprintf(adder_text, "%d + %d = %d\n", *sum, *number, *sum + *number);
    *sum = *sum + *number;
    *number = *number + 1;
    return adder_text;
}

char *get_cpu_usager(long all1, long all2, long idle1, long idle2) {
    char *usage_text = (char *)malloc(sizeof(char) * BUF_LEN);
    float usage;

    char buf[128] = {'\0'};
    char cpu[5];
    long int user,nice,sys,idle,iowait,irq,softirq;

    FILE * fp = fopen("/proc/stat","r");
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

    all2 = user+nice+sys+idle+iowait+irq+softirq;
    idle2 = idle;
    fclose(fp);

    usage = (float)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ;

    sprintf(usage_text, "cpu usage = %.2f%%\n",usage);

    return usage_text;
}