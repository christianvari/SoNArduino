#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "gui/linked_list.h"
#include "./client_packet/client_packet.h"

/**

    Prerequisites:
  
        libgtk-3-dev
        libcairo2-dev

    Compile:

        gcc main.c gui/linked_list.c client_packet/client_packet.c ../packet/packet.c -o main `pkg-config --cflags --libs cairo gtk+-3.0` -lm

 */


#define MAX_RANGE 400
#define DEFAULT_START 80

int fd;

int serial_set_interface_attribs(int fd, int speed, int parity){
    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0){
        perror("Error from tcgetattr");
        return -1;
    }
    switch (speed)
    {
        case 9600:
            speed=B9600;
            break;
        case 19200:
            speed=B19200;
            break;
        case 57600:
            speed=B57600;
            break;
        case 115200:
            speed=B115200;
            break;
        default:
            printf("cannot set baudrate %d\n", speed);
            return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    cfmakeraw(&tty);

    tty.c_cflag &= ~(PARENB|PARODD);
    tty.c_cflag |= parity;
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;

    if(tcsetattr(fd, TCSANOW, &tty) != 0){
        perror("Error from tcsetattr");
        return -1;
    }
    return 0;
}

static void do_drawing(cairo_t *, GtkWidget*);

struct {
  int count;
  double coordx[100];
  double coordy[100];
  ListHead* head;
  double line_width;
} glob;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data){
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

static void do_drawing(cairo_t *cr, GtkWidget *widget){
    GtkWidget *win = gtk_widget_get_toplevel(widget);

    int width, height;
    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    cairo_translate(cr, width/2, height/5*4);
    cairo_rotate (cr, M_PI);

    draw_radar_field(cr);
    draw_all_status_in_list(cr, glob.head, glob.line_width);

}

static gboolean time_handler(GtkWidget *widget){
  gtk_widget_queue_draw(widget);
  
  return TRUE;
}

void* reader_work(void* x) {

    uint8_t byte;
    int bytes_read=0;
    int ret;
    Packet *packet;
    while(1){
        packet = client_receive_packet(fd);
        //client_print_packet(packet);
        if(packet->type==STATUS){
        //FAI ROBA
            List_insert(glob.head, ((StatusPacket*)packet)->angle, ((StatusPacket*)packet)->distance);
        }


        free(packet);
    }
    pthread_exit(NULL);
}

int open_serial(){
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
    if(fd<0){
        perror("Error opening serial");
        return -1;
    }

    int ret = serial_set_interface_attribs(fd, 19200, 0);
    if (ret!=0){
        perror("Error setting serial interface");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *darea;

    glob.count = 0;
    glob.head=malloc(sizeof(ListHead));
    List_init(glob.head, 40);
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

    int ret = open_serial();
    if(ret){
        perror("Error in open_serial");
        exit(EXIT_FAILURE);
    }
    pthread_t reader;
    ret = pthread_create(&reader, NULL, reader_work, NULL);
    if (ret != 0) { fprintf(stderr, "Error %d in pthread_create\n", ret); exit(EXIT_FAILURE); }

    
    
    //need to free list

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}