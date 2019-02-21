//
// Created by vinstarry on 2/18/19.
//

#ifndef MULPROC_DISPLAY_TABLE_OP_H
#define MULPROC_DISPLAY_TABLE_OP_H

#include <gtk/gtk.h>

/* linked list of text */
typedef struct __label_list {
    GtkWidget *lb;
    struct __label_list *next;
}lb_list;

/* cross linked list, row and column respectively */
typedef struct __cross_linked_list {
    lb_list *content_head;
    struct __cross_linked_list *next;
}cross_linked_list;

/* initialize table with title */
void init_table(GtkWidget *table, cross_linked_list *tb_head);
/* add fstab's info table */
void add_fstab_title(lb_list *title_head);
/* fill table according to the label_list */
void fill_table(GtkWidget *table, cross_linked_list *tb_head);
/* get fstab's info, and add it into label list */
void get_fstab_info(cross_linked_list *tb_head);

#endif //MULPROC_DISPLAY_TABLE_OP_H
