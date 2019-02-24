#include "../arduino_packet/arduino_packet.h"
#include <util/delay.h>
#include <stdio.h>

int main(void){

    Packet cpacket;
    cpacket.type = COMMAND;
    CommandPacket commandpacket;
    commandpacket.packet = cpacket;
    commandpacket.command = 1;
    //printf_init();
    UART_init();

    int started=0;
    while(!started){

        arduino_receive_packet(&commandpacket);

        if(commandpacket.packet.type==COMMAND && commandpacket.command == START){
            started=1;
        }
        

    
    }
/*
    printf("type: %d", commandpacket.packet.type);
    printf("command: %d", commandpacket.command);
*/

    StatusPacket statuspacket;
    Packet spacket;
    spacket.type=STATUS;
    statuspacket.packet=spacket;
    statuspacket.angle = 34;
    statuspacket.distance = 45;
    arduino_send_packet((Packet*)(&statuspacket));

    //printf("finito");
}

