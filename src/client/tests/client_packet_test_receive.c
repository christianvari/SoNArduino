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

int serial_set_interface_attribs(int fd, int speed, int parity){
    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0){
        perror("Error from tcgetattr");
        return -1;
    }
    switch (speed)
    {
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


int main(){

    Packet *packet;
    
    //int fd = open("fifo", O_RDONLY);
    
    int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
    if(fd<0){
        perror("Error opening serial");
    }

    int ret = serial_set_interface_attribs(fd, 19200, 0);
    if (ret!=0){
        perror("Error setting serial interface");
    }


    while(1){

        printf("\nWaiting for a new packet\n");


        packet = client_receive_packet(packet, fd);

        //printf("recived");
        client_print_packet(packet);
        free(packet);        
        
    }
}