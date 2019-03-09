#include "../client_packet/client_packet.h"
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>       // nanosleep()

sem_t sem;
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

void* reader_work(void* x) {

    printf("reader\n");

    uint8_t byte;
    int bytes_read=0;
    int ret;
    Packet *packet;
    packet = client_receive_packet(fd);
    printf("packet type: %x\n", packet->type);

    client_print_packet(packet);

    pthread_exit(NULL);
}

void* writer_work(void* x) {

    printf("\t\twriter\n");
    

    Packet packet;
    packet.type=COMMAND;
    CommandPacket commandPacket;
    commandPacket.packet=packet;
    commandPacket.command=START;
    while(1){
        client_send_packet((Packet*)(&commandPacket), fd);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(){

    Packet *packet;
    Packet cmdpacket;
    pthread_t reader, writer;
    
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
    if(fd<0){
        perror("Error opening serial");
    }

    int ret = serial_set_interface_attribs(fd, 19200, 0);
    if (ret!=0){
        perror("Error setting serial interface");
    }

    if (sem_init(&sem, 0, 1)) { fprintf(stderr, "sem_init error\n"); exit(EXIT_FAILURE); } 

    //create threads
    ret = pthread_create(&reader, NULL, reader_work, NULL);
    if (ret != 0) { fprintf(stderr, "Error %d in pthread_create\n", ret); exit(EXIT_FAILURE); }
    
    ret = pthread_create(&writer, NULL, writer_work, NULL);
    if (ret != 0) { fprintf(stderr, "Error %d in pthread_create\n", ret); exit(EXIT_FAILURE); }

    //join on threads
    ret = pthread_join(reader, NULL);
    if (ret != 0) { fprintf(stderr, "Error %d in pthread_join\n", ret); exit(EXIT_FAILURE); }

    ret = pthread_join(writer, NULL);
    if (ret != 0) { fprintf(stderr, "Error %d in pthread_join\n", ret); exit(EXIT_FAILURE); }



    if (sem_destroy(&sem)) { fprintf(stderr, "sem_destroy error\n"); exit(EXIT_FAILURE); } 

    printf("finito\n");

    exit(EXIT_SUCCESS);
}

