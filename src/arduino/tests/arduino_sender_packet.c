#include "../arduino_packet/arduino_packet.h"
#include <util/delay.h>
#include <stdio.h>

int main(void){

    Packet spacket;
    Packet epacket;
    UART_init();

    spacket.type=STATUS;

    StatusPacket statusPacket;
    statusPacket.packet=spacket;
    statusPacket.angle=52;
    statusPacket.distance=1;


    epacket.type=ERROR;

    ErrorPacket errorPacket;

    errorPacket.packet=epacket;
    errorPacket.error_code = OutRange;

    while(1){
        arduino_send_packet((Packet*)(&statusPacket));

        statusPacket.angle++;
        statusPacket.distance++;

        arduino_send_packet((Packet*)(&errorPacket));

        //arduino_print_packet((Packet*)(&statusPacket));

        _delay_ms(1000); // from delay.h, wait 1 sec
    }
}