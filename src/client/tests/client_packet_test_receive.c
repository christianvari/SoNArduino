#include "../client_packet/client_packet.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    Packet *packet;
    int fd = open("fifo", O_RDONLY);

    while(1){

    printf("\nWaiting for a new packet\n");


        packet = client_receive_packet(packet, fd);


        client_print_packet(packet);
        free(packet);        
        //sleep(10);
    }
}