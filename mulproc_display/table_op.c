//
// Created by vinstarry on 2/18/19.
//

#include "table_op.h"

#define ROW_MAX 19
#define COL_MAX 6
#define LINE_MAX_CHARA 1024

void init_table(GtkWidget *table, cross_linked_list *tb_head) {
    if (tb_head) {
        cross_linked_list *cur = tb_head;
        cur->next = (cross_linked_list *)malloc(sizeof(lb_list));
        cur = cur->next;
        cur->content_head = (lb_list *)malloc(sizeof(lb_list));
        add_fstab_title(cur->content_head);
        for (int i = 0; i < ROW_MAX; i++) {
            cur->next = (cross_linked_list *)malloc(sizeof(lb_list));
            cur = cur->next;
            cur->content_head = (lb_list *)malloc(sizeof(lb_list));
            lb_list *ptr = cur->content_head;
            for (int j = 0; j < COL_MAX; j++) {
                ptr->next = (lb_list *)malloc(sizeof(lb_list));
                ptr = ptr->next;
                ptr->lb = gtk_label_new("");
            }
        }
        fill_table(table, tb_head);
    }
}

void add_fstab_title(lb_list *title_head) {
    if (title_head) {
        lb_list *temp = title_head;
        title_head->next = (lb_list *)malloc(sizeof(lb_list));
        temp = title_head->next;
        temp->lb = gtk_label_new("deivce");
        temp->next = (lb_list *)malloc(sizeof(lb_list));
        temp = temp->next;
        temp->lb = gtk_label_new("dir");
        temp->next = (lb_list *)malloc(sizeof(lb_list));
        temp = temp->next;
        temp->lb = gtk_label_new("type");
        temp->next = (lb_list *)malloc(sizeof(lb_list));
        temp = temp->next;
        temp->lb = gtk_label_new("options");
        temp->next = (lb_list *)malloc(sizeof(lb_list));
        temp = temp->next;
        temp->lb = gtk_label_new("dump");
        temp->next = (lb_list *)malloc(sizeof(lb_list));
        temp = temp->next;
        temp->lb = gtk_label_new("pass");
        temp->next = (lb_list *)malloc(sizeof(lb_list));
        temp->next = NULL;
    }
}

void fill_table(GtkWidget *table, cross_linked_list *tb_head) {
    cross_linked_list *cur = tb_head;
    for (int i = 0; i < ROW_MAX; i++) {
        cur = cur->next;
        lb_list *temp = cur->content_head;
        for (int j = 0; j < COL_MAX; j++) {
            temp = temp->next;
            gtk_table_attach(GTK_TABLE(table), temp->lb, (guint)j, (guint)j+1, (guint)i, (guint)i+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        }
    }
}

void get_fstab_info(cross_linked_list *tb_head) {
    FILE *fp = fopen("/etc/fstab", "r");
    char temp[LINE_MAX_CHARA];
    if (fp) {
        cross_linked_list *cur = tb_head->next;
        cur = cur->next;
        while ((fgets(temp, LINE_MAX_CHARA, fp)) != NULL) {
            if (temp[0] == '#')
                continue;
            else {
                cur = cur->next;
                lb_list *temp_ptr = cur->content_head;
                char dev[200], dir[200], type[20], options[50];
                char dump[2], pass[2];
                if (6 != (sscanf(temp, "%s %s %s %s %s %s", dev, dir, type, options, dump, pass)))
                    break;
                temp_ptr = temp_ptr->next;
                gtk_label_set_text(GTK_LABEL(temp_ptr->lb), dev);
                temp_ptr = temp_ptr->next;
                gtk_label_set_text(GTK_LABEL(temp_ptr->lb), dir);
                temp_ptr = temp_ptr->next;
                gtk_label_set_text(GTK_LABEL(temp_ptr->lb), type);
                temp_ptr = temp_ptr->next;
                gtk_label_set_text(GTK_LABEL(temp_ptr->lb), options);
                temp_ptr = temp_ptr->next;
                gtk_label_set_text(GTK_LABEL(temp_ptr->lb), dump);
                temp_ptr = temp_ptr->next;
                gtk_label_set_text(GTK_LABEL(temp_ptr->lb), pass);
            }
        }
    }
    fclose(fp);
}