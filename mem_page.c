//
// Created by vinstarry on 2/20/19.
//

#include "mem_page.h"

#define VALUE_MAX_LEN 100

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