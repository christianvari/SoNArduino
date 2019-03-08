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


#define MAX_RANGE 400
#define DEFAULT_START 80

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
  
  cairo_translate(cr, width/2, height/5*4);
  cairo_rotate (cr, M_PI);

  cairo_move_to (cr, 0, DEFAULT_START);
  cairo_line_to (cr, 0, MAX_RANGE);
  cairo_stroke(cr);  

  cairo_move_to (cr, DEFAULT_START*cos(M_PI/3), DEFAULT_START*sin(M_PI/3));
  cairo_line_to (cr, MAX_RANGE*cos(M_PI/3), MAX_RANGE*sin(M_PI/3));
  cairo_stroke(cr);

  cairo_move_to (cr, DEFAULT_START*cos(M_PI/6), DEFAULT_START*sin(M_PI/6));
  cairo_line_to (cr, MAX_RANGE*cos(M_PI/6), MAX_RANGE*sin(M_PI/6));
  cairo_stroke(cr);

  cairo_move_to (cr, DEFAULT_START*cos(M_PI/6*4), DEFAULT_START*sin(M_PI/6*4));
  cairo_line_to (cr, MAX_RANGE*cos(M_PI/6*4), MAX_RANGE*sin(M_PI/6*4));
  cairo_stroke(cr);

  cairo_move_to (cr, DEFAULT_START*cos(M_PI/6*5), DEFAULT_START*sin(M_PI/6*5));
  cairo_line_to (cr, MAX_RANGE*cos(M_PI/6*5), MAX_RANGE*sin(M_PI/6*5));
  cairo_stroke(cr);

  cairo_move_to (cr, DEFAULT_START*cos(0), DEFAULT_START*sin(0));
  cairo_line_to (cr, MAX_RANGE*cos(0), MAX_RANGE*sin(0));
  cairo_stroke(cr);

  cairo_move_to (cr, DEFAULT_START*cos(M_PI), DEFAULT_START*sin(M_PI));
  cairo_line_to (cr, MAX_RANGE*cos(M_PI), MAX_RANGE*sin(M_PI));
  cairo_stroke(cr);

  cairo_arc(cr, 0, 0, MAX_RANGE, 0, M_PI);
  cairo_stroke(cr);  
  cairo_arc(cr, 0, 0, MAX_RANGE/4*3, 0, M_PI);
  cairo_stroke(cr);  
  cairo_arc(cr, 0, 0, MAX_RANGE/2, 0, M_PI);
  cairo_stroke(cr);  
  cairo_arc(cr, 0, 0, MAX_RANGE/4, 0, M_PI);
  cairo_stroke(cr);   

  cairo_set_line_width(cr, 10);  
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_source_rgb(cr, 1, 0, 0);

  cairo_move_to (cr, DEFAULT_START*cos(M_PI/180*1), DEFAULT_START*sin(M_PI/180*1));
  cairo_line_to (cr, MAX_RANGE*cos(M_PI/180*1), MAX_RANGE*sin(M_PI/180*1));
  cairo_stroke(cr);
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
   
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 900, 600); 
  gtk_window_set_title(GTK_WINDOW(window), "Sonar test");

  gtk_widget_show_all(window);



  gtk_main();

  

  return 0;
}