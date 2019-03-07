#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>
#include "gui/linked_list.h"

/**

    Prerequisites:
  
        libgtk-3-dev
        libcairo2-dev

    Compile:

        gcc main.c gui/linked_list.c -o main `pkg-config --cflags --libs cairo gtk+-3.0` -lm

 */


#define MAX_RANGE 400
#define DEFAULT_START 80

static void do_drawing(cairo_t *, GtkWidget*);

struct {
  int count;
  double coordx[100];
  double coordy[100];
  ListHead* head;
  double line_width;
} glob;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
  do_drawing(cr, widget);
  return FALSE;
}



void draw_line(cairo_t *cr, uint8_t angle, uint8_t distance, double transparence){

    cairo_set_source_rgba(cr, 1, 0, 0, transparence);

    cairo_move_to (cr, DEFAULT_START*cos(M_PI/180*angle), DEFAULT_START*sin(M_PI/180*angle));
    cairo_line_to (cr, MAX_RANGE*cos(M_PI/180*angle), MAX_RANGE*sin(M_PI/180*angle));
    cairo_stroke(cr);

}

void draw_all_status_in_list(cairo_t *cr, ListHead *head, double line_width){

    cairo_set_line_width(cr, line_width);  
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    ListItem* aux=head->first;
    int i=0;
    int tot=head->max_length;
    while(aux){
        draw_line(cr, aux->angle, aux->distance, 1-((1.0/tot)*i));
        i++;
        aux = aux->next;
    }
}

void draw_radar_field(cairo_t *cr){

    cairo_set_line_width(cr, 0.5);  
    cairo_set_source_rgb(cr, 0, 0, 0);

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
}


static void do_drawing(cairo_t *cr, GtkWidget *widget)
{
    GtkWidget *win = gtk_widget_get_toplevel(widget);

    int width, height;
    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    cairo_translate(cr, width/2, height/5*4);
    cairo_rotate (cr, M_PI);

    draw_radar_field(cr);
    draw_all_status_in_list(cr, glob.head, glob.line_width);

}



static gboolean time_handler(GtkWidget *widget)
{
  gtk_widget_queue_draw(widget);
  
  return TRUE;
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *darea;

    glob.count = 0;
    glob.head=malloc(sizeof(ListHead));
    List_init(glob.head, 20);
    glob.line_width = 10;

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

    //refresh 10 times per sec (100millisec)
    g_timeout_add(100, (GSourceFunc) time_handler, (gpointer) window);


/*--------  START LIST TEST  --------*/
    ListItem* i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=1;
    i->distance=1;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=2;
    i->distance=2;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=3;
    i->distance=3;
    List_insert(glob.head, i);
    
    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=4;
    i->distance=4;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=5;
    i->distance=5;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=6;
    i->distance=6;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=7;
    i->distance=1;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=7;
    i->distance=2;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=8;
    i->distance=3;
    List_insert(glob.head, i);
    
    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=9;
    i->distance=4;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=10;
    i->distance=5;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=11;
    i->distance=6;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=12;
    i->distance=1;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=13;
    i->distance=2;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=14;
    i->distance=3;
    List_insert(glob.head, i);
    
    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=15;
    i->distance=4;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=16;
    i->distance=5;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=17;
    i->distance=6;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=18;
    i->distance=1;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=19;
    i->distance=2;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=20;
    i->distance=3;
    List_insert(glob.head, i);
    
    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=21;
    i->distance=4;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=22;
    i->distance=5;
    List_insert(glob.head, i);

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=23;
    i->distance=6;
    List_insert(glob.head, i);
    
    //need to free list

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}