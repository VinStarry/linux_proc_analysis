#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>

/* Restart dialog responses.
 */
typedef enum
{
    RESPONSE_RESTART,
    RESPONSE_CANCEL
} Resp;

/* Application state.
 */
typedef struct _App
{
    /* Background thread.
     */
    GThread *bg_thr;

    /* Label we update with progress.
     */
    GtkWidget *status;

    /* Main window.
     */
    GtkWidget *win;
} App;

/* A message from the worker.
 */
typedef struct _Msg
{
    App *app;
    char *text;
} Msg;

static gboolean
message_idle (Msg * msg)
{
    gtk_label_set_text (GTK_LABEL (msg->app->status), msg->text);

    g_free (msg->text);
    g_free (msg);

    return FALSE;
}

static void *background_thr (void *data);

static void
start_worker (App * app)
{
    if (!app->bg_thr)
        app->bg_thr = g_thread_new ("worker", background_thr, app);
}

static gboolean
alert_idle (App * app)
{
    GtkWidget *dia;
    GtkWidget *lab;
    Resp resp;

    dia = gtk_dialog_new_with_buttons ("Message", GTK_WINDOW (app->win),
                                       GTK_DIALOG_DESTROY_WITH_PARENT |
                                       GTK_DIALOG_MODAL,
                                       "Restart", RESPONSE_RESTART,
                                       "Cancel", RESPONSE_CANCEL, NULL);
    lab = gtk_label_new ("Error: restart worker?");
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dia)->vbox), lab);
    gtk_widget_show_all (dia);
    resp = gtk_dialog_run (GTK_DIALOG (dia));
    gtk_widget_destroy (dia);

    if (resp == RESPONSE_RESTART)
        start_worker (app);

    return FALSE;
}

static gboolean
done_idle (App * app)
{
    app->bg_thr = NULL;

    gtk_label_set_text (GTK_LABEL (app->status), "idle");

    return FALSE;
}

static void *
background_thr (void *data)
{
    App *app = (App *) data;
    int i;
    Msg *msg;

    for (i = 0; i < 10; i++)
    {
        msg = g_new (Msg, 1);
        msg->app = app;
        msg->text = g_strdup_printf ("working %d ..", i);
        g_idle_add ((GSourceFunc) message_idle, msg);

        if ((rand () & 255) > 200)
        {
            g_idle_add ((GSourceFunc) alert_idle, app);
            break;
        }

        sleep (1);
    }

    g_idle_add ((GSourceFunc) done_idle, app);

    return (0);
}

static void
click_cb (GtkWidget * but, App * app)
{
    start_worker (app);
}

int
main (int argc, char **argv)
{
    App app;
    GtkWidget *vbox;
    GtkWidget *but;

    gtk_init (&argc, &argv);

    app.bg_thr = NULL;

    app.win = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* Ought to kill the bg thread too I guess.
     */
    g_signal_connect (app.win, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    vbox = gtk_vbox_new (FALSE, 2);
    gtk_container_add (GTK_CONTAINER (app.win), vbox);
    but = gtk_button_new_with_label ("Start bg thread");
    g_signal_connect (but, "clicked", G_CALLBACK (click_cb), &app);
    gtk_container_add (GTK_CONTAINER (vbox), but);
    app.status = gtk_label_new ("idle");
    gtk_container_add (GTK_CONTAINER (vbox), app.status);

    gtk_widget_show_all (app.win);

    gtk_main ();

    return 0;
}