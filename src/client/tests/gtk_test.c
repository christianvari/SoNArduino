#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>


/**

    Prerequisites:
  
        libgtk-3-dev
        libcairo2-dev

    Compile:

        gcc gtk_test.c -o gtk_test `pkg-config --cflags --libs cairo gtk+-3.0` -lm

 */



static void do_drawing(cairo_t *, GtkWidget*);

struct {
  int count;
  double coordx[100];
  double coordy[100];
} glob;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
  do_drawing(cr, widget);
  return FALSE;
}

static void do_drawing(cairo_t *cr, GtkWidget *widget)
{
  GtkWidget *win = gtk_widget_get_toplevel(widget);
  
  int width, height;
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);
  
  cairo_set_line_width(cr, 0.5);  
  cairo_set_source_rgb(cr, 0, 0, 0);
  
  cairo_translate(cr, width/2, height/4*3);
  cairo_rotate (cr, M_PI);

  cairo_move_to (cr, 0, 80);
  cairo_line_to (cr, 0, 400);
  cairo_stroke(cr);  

  cairo_move_to (cr, 80*cos(M_PI/3), 80*sin(M_PI/3));
  cairo_line_to (cr, 400*cos(M_PI/3), 400*sin(M_PI/3));
  cairo_stroke(cr);

  cairo_move_to (cr, 80*cos(M_PI/6), 80*sin(M_PI/6));
  cairo_line_to (cr, 400*cos(M_PI/6), 400*sin(M_PI/6));
  cairo_stroke(cr);

  cairo_move_to (cr, 80*cos(M_PI/6*4), 80*sin(M_PI/6*4));
  cairo_line_to (cr, 400*cos(M_PI/6*4), 400*sin(M_PI/6*4));
  cairo_stroke(cr);

  cairo_move_to (cr, 80*cos(M_PI/6*5), 80*sin(M_PI/6*5));
  cairo_line_to (cr, 400*cos(M_PI/6*5), 400*sin(M_PI/6*5));
  cairo_stroke(cr);

  cairo_move_to (cr, 80*cos(0), 80*sin(0));
  cairo_line_to (cr, 400*cos(0), 400*sin(0));
  cairo_stroke(cr);

  cairo_move_to (cr, 80*cos(M_PI), 80*sin(M_PI));
  cairo_line_to (cr, 400*cos(M_PI), 400*sin(M_PI));
  cairo_stroke(cr);

  cairo_arc(cr, 0, 0, 400, 0, M_PI);
  cairo_stroke(cr);  
  cairo_arc(cr, 0, 0, 300, 0, M_PI);
  cairo_stroke(cr);  
  cairo_arc(cr, 0, 0, 100, 0, M_PI);
  cairo_stroke(cr);  
  cairo_arc(cr, 0, 0, 200, 0, M_PI);
  cairo_stroke(cr);   
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    printf("event->button: %d\n", event->button);
    if (event->button == 1) {
        glob.coordx[glob.count] = event->x;
        glob.coordy[glob.count++] = event->y;
    }

    if (event->button == 3) {
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
}


int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  
  glob.count = 0;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);
 
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL); 
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  
    
  //g_signal_connect(window, "button-press-event", G_CALLBACK(clicked), NULL);
 
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 900, 600); 
  gtk_window_set_title(GTK_WINDOW(window), "Lines");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}