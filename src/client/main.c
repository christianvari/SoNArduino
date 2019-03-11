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
#include <time.h>
#include "gui/linked_list.h"
#include "./client_packet/client_packet.h"

/**

    Prerequisites:
  
        libgtk-3-dev
        libcairo2-dev

    Compile:

        gcc main.c gui/linked_list.c client_packet/client_packet.c ../packet/packet.c -o main `pkg-config --cflags --libs cairo gtk+-3.0` -lm

 */



int fd;
GtkWidget *darea;
int MAX_RANGE = 400;
int DEFAULT_START = 80;
int DEFAULT_RANGE = 60;
pthread_t reader;
GtkWidget *start_button, *stop_button, *connect_button, *disconnect_button;
GtkWidget *accuracy_scale, *accuracy_label;
GtkWidget *speed_scale, *speed_label;
GtkWidget *range_scale, *range_label;
int connected = 0;
int started = 0;
int configured = 0;
int arduino_speed=1, arduino_accuracy=1;
int client_speed=1, client_accuracy=1;
int range;
char un_quarto[5] = "15", due_quarti[5]="30", tre_quarti[5]="45", quattro_quarti[5]="60";


/**================================================== *
 * ==========  SERIAL FUNCTIONS  ========== *
 * ================================================== */

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

int open_serial(){
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
    if(fd<0){
        perror("Error opening serial");
        return -1;
    }

    int ret = serial_set_interface_attribs(fd, 57600, 0);
    if (ret!=0){
        perror("Error setting serial interface");
        return -1;
    }
    return 0;
}

/**================================================== *
 * ==========  DRAWING FUNCTIONS  ========== *
 * ================================================== */

static void do_drawing(cairo_t *, GtkWidget*);

struct {
  int count;
  double coordx[100];
  double coordy[100];
  ListHead* head;
  double line_width;
} glob;

void draw_line(cairo_t *cr, uint8_t angle, uint8_t distance, double transparence){

    cairo_set_source_rgba(cr, 1, 0, 0, transparence);

    //proportion distance:256=d:MAX_RANGE

    // d:range=x:256
    if(distance<=range){
        int x = ((double)(distance*256))/range;
    
        int d = (double)x*MAX_RANGE/256;
    
        cairo_move_to (cr, -d*cos(M_PI/180*angle), d*sin(M_PI/180*angle));
        cairo_line_to (cr, -MAX_RANGE*cos(M_PI/180*angle), MAX_RANGE*sin(M_PI/180*angle));
        cairo_stroke(cr);
    }
    else{
        cairo_move_to (cr, -MAX_RANGE*cos(M_PI/180*angle), MAX_RANGE*sin(M_PI/180*angle));
        cairo_line_to (cr, -MAX_RANGE*cos(M_PI/180*angle), MAX_RANGE*sin(M_PI/180*angle));
        cairo_stroke(cr);
    }
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

    cairo_rotate (cr, M_PI);
    cairo_move_to (cr, (double)MAX_RANGE/4, 20);
    cairo_show_text (cr, un_quarto);
    cairo_move_to (cr, (double)MAX_RANGE/4*2, 20);
    cairo_show_text (cr, due_quarti);
    cairo_move_to (cr, (double)MAX_RANGE/4*3, 20);
    cairo_show_text (cr, tre_quarti);
    cairo_move_to (cr, (double)MAX_RANGE, 20);
    cairo_show_text (cr, quattro_quarti);
    cairo_rotate (cr, M_PI);

}

static void do_drawing(cairo_t *cr, GtkWidget *widget){
    GtkWidget *win = gtk_widget_get_toplevel(widget);

    int width, height;
    width = gtk_widget_get_allocated_width(darea);
    height = gtk_widget_get_allocated_height(darea);
    //gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    MAX_RANGE=(int)((double)width/9.0*4);
    if(MAX_RANGE>(double)height/5*3) MAX_RANGE=(int)((double)height/5*3);
    DEFAULT_START=(int)((double)MAX_RANGE/5.0);

    cairo_translate(cr, width/2, height/5*4);
    cairo_rotate (cr, M_PI);

    draw_radar_field(cr);
    draw_all_status_in_list(cr, glob.head, glob.line_width);

}

/**================================================== *
 * ==========  THREADS WORK  ========== *
 * ================================================== */

void* reader_work(void* x) {

    uint8_t byte;
    int bytes_read=0;
    int ret;
    Packet *packet;
    while(connected){

        packet = client_receive_packet(fd);
        //client_print_packet(packet);
        if(packet->type==STATUS){
        //FAI ROBA
            List_insert(glob.head, ((StatusPacket*)packet)->angle, ((StatusPacket*)packet)->distance);
        }else if(packet->type==EVENT){
            switch (((EventPacket*)packet)->event)
            {
                case STARTED:
                    printf("start received\n");
                    started = 1;
                    break;

                case STOPPED:
                    printf("stop received\n");
                    started = 0;
                    List_reset(glob.head);
                    break;

                case DISCONNECTED:
                    printf("disconnect received\n");
                    connected = 0;
                    break;
                default:
                    printf("UnKnown Event received\n");
                    break;
            }
        }else if(packet->type == CONFIGURATION){
            
            arduino_speed = ((ConfigurationPacket*)packet)->velocity;
            arduino_accuracy = ((ConfigurationPacket*)packet)->precision;
            configured = 1;
            printf("Configured\n");

        }else if(packet->type == ERROR){
            printf("Error received: error code %d\n", ((ErrorPacket*)(packet))->error_code);
        }


        free(packet);
    }
    pthread_exit(NULL);
}

/**================================================== *
 * ==========  EVENT HANDLER  ========== *
 * ================================================== */

static void connect_handler(GtkWidget *widget, gpointer data){

    int ret = open_serial();
    if(ret){
        perror("Error in open_serial");
        return;
    }
    connected=1;
    ret = pthread_create(&reader, NULL, reader_work, NULL);
    if (ret != 0) { fprintf(stderr, "Error %d in pthread_create\n", ret); exit(EXIT_FAILURE); }
    

    Packet packet;
    packet.type = COMMAND;
    CommandPacket commandPacket;
    commandPacket.packet = packet;
    commandPacket.command = SEND_CONFIG;
    commandPacket.payload = 0;

    usleep(1000000);

    struct timespec str;
    str.tv_sec=0;
    str.tv_nsec=350000000;
    while(!configured){

        client_send_packet((Packet*)(&commandPacket), fd);

        nanosleep(&str, NULL);

    }
    
    printf("Connected\n");

    gtk_widget_set_sensitive (connect_button, FALSE);
    gtk_widget_set_sensitive (disconnect_button, TRUE);
    gtk_widget_set_sensitive (start_button, TRUE);
    gtk_widget_set_sensitive (stop_button, FALSE);
    gtk_widget_set_sensitive (accuracy_scale, TRUE);
    gtk_range_set_value((GtkRange*)accuracy_scale, arduino_accuracy);
    gtk_widget_set_sensitive (speed_scale, TRUE);
    gtk_range_set_value((GtkRange*)speed_scale, arduino_speed);
    gtk_widget_set_sensitive (range_scale, TRUE);

}

static void start_handler(GtkWidget *widget, gpointer data){
    Packet packet;
    packet.type = COMMAND;
    CommandPacket commandPacket;
    commandPacket.packet = packet;
    commandPacket.command = START;
    commandPacket.payload = 0;


    struct timespec str;
    str.tv_sec=0;
    str.tv_nsec=350000000;

    if(client_accuracy != arduino_accuracy || client_speed != arduino_speed){
        configured=0;
    }
    while(!configured){
        if(client_accuracy != arduino_accuracy){
            Packet packet;
            packet.type = COMMAND;
            CommandPacket commandPacket;
            commandPacket.packet = packet;
            commandPacket.command = SET_PRECISION;
            commandPacket.payload = client_accuracy;
            client_send_packet((Packet*)(&commandPacket), fd);
        }
        if(client_speed != arduino_speed){
            Packet packet;
            packet.type = COMMAND;
            CommandPacket commandPacket;
            commandPacket.packet = packet;
            commandPacket.command = SET_VELOCITY;
            commandPacket.payload = client_speed;
            client_send_packet((Packet*)(&commandPacket), fd);
        }
        if(client_accuracy == arduino_accuracy && client_speed == arduino_speed){
            configured=1;
        }
    }
    

    while(!started){

        client_send_packet((Packet*)(&commandPacket), fd);

        nanosleep(&str, NULL);

    }

    gtk_widget_set_sensitive (connect_button, FALSE);
    gtk_widget_set_sensitive (disconnect_button, FALSE);
    gtk_widget_set_sensitive (start_button, FALSE);
    gtk_widget_set_sensitive (stop_button, TRUE);
    gtk_widget_set_sensitive (accuracy_scale, FALSE);
    gtk_widget_set_sensitive (speed_scale, FALSE);
    gtk_widget_set_sensitive (range_scale, FALSE);

}

static void stop_handler(GtkWidget *widget, gpointer data){
    Packet packet;
    packet.type = COMMAND;
    CommandPacket commandPacket;
    commandPacket.packet = packet;
    commandPacket.command = STOP;
    commandPacket.payload = 0;

    struct timespec str;
    str.tv_sec=0;
    str.tv_nsec=350000000;
    while(started){

        client_send_packet((Packet*)(&commandPacket), fd);

        nanosleep(&str, NULL);

    }

    List_reset(glob.head);

    gtk_widget_set_sensitive (connect_button, FALSE);
    gtk_widget_set_sensitive (disconnect_button, TRUE);
    gtk_widget_set_sensitive (start_button, TRUE);
    gtk_widget_set_sensitive (stop_button, FALSE);
    gtk_widget_set_sensitive (accuracy_scale, TRUE);
    gtk_widget_set_sensitive (speed_scale, TRUE);
    gtk_widget_set_sensitive (range_scale, TRUE);

}

static void disconnect_handler(GtkWidget *widget, gpointer data){

    Packet packet;
    packet.type = COMMAND;
    CommandPacket commandPacket;
    commandPacket.packet = packet;
    commandPacket.command = DISCONNECTION;
    commandPacket.payload = 0;

    struct timespec str;
    str.tv_sec=0;
    str.tv_nsec=300000000;
    while(connected){

        client_send_packet((Packet*)(&commandPacket), fd);

        nanosleep(&str, NULL);

    }

    pthread_join(reader, NULL);

    close(fd);

    gtk_widget_set_sensitive (connect_button, TRUE);
    gtk_widget_set_sensitive (disconnect_button, FALSE);
    gtk_widget_set_sensitive (start_button, FALSE);
    gtk_widget_set_sensitive (stop_button, FALSE);
    gtk_widget_set_sensitive (accuracy_scale, FALSE);
    gtk_widget_set_sensitive (speed_scale, FALSE);
    gtk_widget_set_sensitive (range_scale, FALSE);
  
}

static void accuracy_handler(GtkWidget *widget, gpointer data){

    int pos = gtk_range_get_value((GtkRange*)accuracy_scale);
    client_accuracy = pos;

}

static void speed_handler(GtkWidget *widget, gpointer data){

    int pos = gtk_range_get_value((GtkRange*)speed_scale);
    client_speed = pos;

}

static void range_handler(GtkWidget *widget, gpointer data){

    range = gtk_range_get_value((GtkRange*)range_scale);
    //change range and labels

    int uno = ((double)range/4)+0.5;
    int due = ((double)range/4*2)+0.5;
    int tre = ((double)range/4*3)+0.5;
    int quattro = ((double)range)+0.5;

    sprintf(un_quarto, "%d", uno);
    sprintf(due_quarti, "%d", due);
    sprintf(tre_quarti, "%d", tre);
    sprintf(quattro_quarti, "%d", quattro);
}

static gboolean time_handler(GtkWidget *widget){
  gtk_widget_queue_draw(widget);
  
  return TRUE;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data){
  do_drawing(cr, widget);
  return FALSE;
}


/**================================================== *
 * ==========  MAIN  ========== *
 * ================================================== */

int main(int argc, char *argv[])
{
    GtkWidget *window, *main_box, *left_box, *first_row, *second_row, *third_row, *fourth_row, *fifth_row;
    

    glob.count = 0;
    glob.head=malloc(sizeof(ListHead));
    List_init(glob.head, 60);
    glob.line_width = 10;
    range = DEFAULT_RANGE;
    
    gtk_init(&argc, &argv);


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    main_box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    darea = gtk_drawing_area_new();
    left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    first_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    second_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    third_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    fourth_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    fifth_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);


    connect_button = gtk_button_new_with_label ("CONNECT");
    g_signal_connect (connect_button, "clicked", G_CALLBACK (connect_handler), NULL);
    disconnect_button = gtk_button_new_with_label ("DISCONNECT");
    g_signal_connect (disconnect_button, "clicked", G_CALLBACK (disconnect_handler), NULL);
    gtk_widget_set_sensitive (disconnect_button, FALSE);
    gtk_box_pack_start((GtkBox*)first_row, connect_button, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)first_row, disconnect_button, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)left_box, first_row, TRUE, TRUE, 30);

    start_button = gtk_button_new_with_label ("START");
    gtk_widget_set_sensitive (start_button, FALSE);
    g_signal_connect (start_button, "clicked", G_CALLBACK (start_handler), NULL);
    stop_button = gtk_button_new_with_label ("STOP");
    gtk_widget_set_sensitive (stop_button, FALSE);
    g_signal_connect (stop_button, "clicked", G_CALLBACK (stop_handler), NULL);

    gtk_box_pack_start((GtkBox*)second_row, start_button, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)second_row, stop_button, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)left_box, second_row, TRUE, TRUE, 30);

    speed_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 1, 10, 1);
    speed_label = gtk_label_new ("Speed");
    gtk_widget_set_sensitive (speed_scale, FALSE);
    g_signal_connect (speed_scale, "value-changed", G_CALLBACK (speed_handler),  NULL);
    gtk_box_pack_start((GtkBox*)third_row, speed_scale, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)third_row, speed_label, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)left_box, third_row, TRUE, TRUE, 30);

    accuracy_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 1, 5, 1);
    accuracy_label = gtk_label_new ("Accuracy");
    gtk_widget_set_sensitive (accuracy_scale, FALSE);
    g_signal_connect (accuracy_scale, "value-changed", G_CALLBACK (accuracy_handler),  NULL);
    gtk_box_pack_start((GtkBox*)fourth_row, accuracy_scale, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)fourth_row, accuracy_label, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)left_box, fourth_row, TRUE, TRUE, 30);

    range_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 20, 256, 4);
    range_label = gtk_label_new ("Range (cm)");
    gtk_widget_set_sensitive (range_scale, FALSE);
    gtk_range_set_value((GtkRange*)range_scale, DEFAULT_RANGE);
    g_signal_connect (range_scale, "value-changed", G_CALLBACK (range_handler),  NULL);
    gtk_box_pack_start((GtkBox*)fifth_row, range_scale, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)fifth_row, range_label, TRUE, TRUE, 30);
    gtk_box_pack_start((GtkBox*)left_box, fifth_row, TRUE, TRUE, 30);

    gtk_box_pack_start((GtkBox*)main_box, darea, TRUE, TRUE, 0);
    gtk_box_pack_end((GtkBox*)main_box, left_box, TRUE, TRUE, 0);
    
    gtk_container_add(GTK_CONTAINER(window), main_box);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL); 
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 1400, 600); 
    gtk_window_set_title(GTK_WINDOW(window), "Sonar test");

    //refresh 10 times per sec (100millisec)
    g_timeout_add(100, (GSourceFunc) time_handler, (gpointer) window);

    
    //need to free list

    gtk_widget_show_all(window);

    gtk_main();


    List_reset(glob.head);
    free(glob.head);
    return 0;
}