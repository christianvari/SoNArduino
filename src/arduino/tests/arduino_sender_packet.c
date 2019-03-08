#include "../arduino_packet/uart.h"
#include <util/delay.h>


#define DESTRA 0
#define SINISTRA 1

int main(void){

    Packet spacket;
    Packet epacket;
    UART_init();
    Packet spacket;
    spacket.type=STATUS;

    StatusPacket statusPacket;
    statusPacket.packet=spacket;
    statusPacket.angle=0;
    statusPacket.distance=0;

    Packet cpacket;
    cpacket.type = COMMAND;
    CommandPacket commandpacket;
    commandpacket.packet = cpacket;
    commandpacket.command = 99;

    int verso=DESTRA;
    while(1){

        if(arduino_receive_packet(&commandpacket)){

            statusPacket.angle = 44;
            arduino_send_packet((Packet*)(&statusPacket));
        }

        _delay_ms(500); // from delay.h, wait 1 sec
    }
}