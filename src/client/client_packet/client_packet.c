#include "client_packet.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>




int write_buf(int fd, uint8_t* buf, int n){
    int ret;
    int bytes_sent = 0;
    int i;
    for (i=0; i<n; i++){
        bytes_sent=0;
        while(bytes_sent < 1){
            ret = write(fd, buf+i, 1);
            if (ret == -1 && errno == EINTR) continue;
            if (ret == -1) perror("Error writing");
            bytes_sent += ret;
        }
        usleep(1000); //Arduino latency
    }
    for(i=0; i<n; i++){
        printf("%x ", buf[i]);
    }
    printf("\n");
    return bytes_sent;
}


int client_send_packet(Packet* packet, int fd){
    int type = packet->type;
    
    uint8_t buf[52]={0};
    buf[0]=0xaa;
    buf[1]=0x55;
    uint8_t n=0;

    int i;
    switch (type){
        case COMMAND:
            n=sizeof(CommandPacket);
            for(i=0; i<n; i++){
                buf[i+2] = *(((uint8_t*) packet)+i);
            }
            break;

        // client never sends a status packet
        //should client send an error packet??
        //client doesn't send Events and Configurations
        
        default:
            break;
    }

    buf[n+2] = calculate_checksum(buf, n+2);  //header included


    int ret = write_buf(fd, buf, n+3);
    return ret;
}

Packet* client_receive_packet(int fd){
    unsigned char buf[52]={0};
    
    // riguarda per togliere i goto
    int ret;
    A:
    ret = read(fd, buf, 1);
    if (ret == -1 && errno != EINTR) perror("Error reading");
    if (ret == -1 && errno == EINTR) goto A;

    //printf("%x ", buf[0]);
    if(buf[0] != 0xaa) goto A;
    //printf("received[0]: %x\n", buf[0]);

    B:
    ret = read(fd, buf+1, 1);
    if (ret == -1 && errno != EINTR) perror("Error reading");
    if (ret == -1 && errno == EINTR) goto B;

    if(buf[1] == 0xaa) goto B;
    if(buf[1] != 0x55) goto A;
    //printf("received[1]: %x\n", buf[1]);


    C:
    ret = read(fd, buf+2, 1);
    if (ret == -1 && errno != EINTR) perror("Error reading");
    if (ret == -1 && errno == EINTR) goto C;

    //printf("received[2]: %x\n", buf[2]);

    unsigned char count=buf[2];

    int i;
    for(i=0; i<count; i++){
        ret = 0;
        while(ret <= 0){
            ret = read(fd, buf+3+i, 1);
            if (ret == -1 && errno != EINTR) perror("Error reading");
            if (ret == -1 && errno == EINTR) continue;
        }
        //printf("received[%d]: %x\n", 3+i, buf[3+i]);

    }

    D:
    ret = read(fd, buf+3+count, 1);
    if (ret == -1 && errno != EINTR) perror("Error reading");
    if (ret == -1 && errno == EINTR) goto D;

    //printf("received[%d]: %x\n", 3+count ,buf[3+count]);
 
    unsigned char cs = calculate_checksum(buf, count+3);

    if(cs != buf[count+3]){
        printf("Packet received corrupted, start again\n");
        goto A;
    }

    //dentro a buf c'è il pacchetto con head e check
    Packet *packet;
    switch (buf[3])
    {
        case COMMAND:

            packet = (Packet*)malloc(sizeof(CommandPacket));
            packet->type = ((CommandPacket*)(buf+3))->packet.type;
            ((CommandPacket*)packet)->command = ((CommandPacket*)(buf+3))->command;

            break;
        case STATUS:

            packet = (Packet*)malloc(sizeof(StatusPacket));
            packet->type = ((StatusPacket*)(buf+3))->packet.type;
            ((StatusPacket*)packet)->angle = ((StatusPacket*)(buf+3))->angle;
            ((StatusPacket*)packet)->distance = ((StatusPacket*)(buf+3))->distance;

            break;
        case ERROR:

            packet = (Packet*)malloc(sizeof(ErrorPacket));
            packet->type = ((ErrorPacket*)(buf+3))->packet.type;
            ((ErrorPacket*)packet)->error_code = ((ErrorPacket*)(buf+3))->error_code;
            
            break;
        case EVENT:

            packet = (Packet*)malloc(sizeof(EventPacket));
            packet->type = ((EventPacket*)(buf+3))->packet.type;
            ((EventPacket*)packet)->event = ((EventPacket*)(buf+3))->event;
        
            break; 
        case CONFIGURATION:

            packet = (Packet*)malloc(sizeof(ConfigurationPacket));
            packet->type = ((ConfigurationPacket*)(buf+3))->packet.type;
            ((ConfigurationPacket*)packet)->precision = ((ConfigurationPacket*)(buf+3))->precision;
            ((ConfigurationPacket*)packet)->velocity = ((ConfigurationPacket*)(buf+3))->velocity;

            break; 
        
        default:
            printf("packet code unknown: %d\n", buf[3]);
            return NULL;
            break;
    }

    return packet;

}

void client_print_packet(Packet* packet){
    
    switch (packet ->type)
    {
        case COMMAND:
            /* code */
            printf("CommandPacket %d : command %d\n", packet->type, ((CommandPacket*)packet)->command);
            break;

        case STATUS:
            printf("StatusPacket %d : distance %d, angle %d\n", packet->type, ((StatusPacket*)packet)->distance, ((StatusPacket*)packet)->angle);
            break;

        case ERROR:
            printf("ErrorPacket %d : error %d\n", packet->type, ((ErrorPacket*)packet)->error_code);
            break;

        default:
            printf("PacketCode Unknown\n");
            break;
    }
}