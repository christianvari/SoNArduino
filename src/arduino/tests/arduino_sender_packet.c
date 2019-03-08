#include "../arduino_packet/arduino_packet.h"
#include <util/delay.h>


#define DESTRA 0
#define SINISTRA 1

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

    int verso=DESTRA;
    while(1){
        arduino_send_packet((Packet*)(&statusPacket));

        if(verso==DESTRA && statusPacket.angle<180){
            statusPacket.angle++;
            statusPacket.distance++;
        }else if(verso==SINISTRA && statusPacket.angle>0){
            statusPacket.angle--;
            statusPacket.distance--;
        }else if(verso==DESTRA && statusPacket.angle>=180){
            statusPacket.angle--;
            statusPacket.distance--;
            verso=SINISTRA;
        }else if(verso==SINISTRA && statusPacket.angle<=0){
            statusPacket.angle++;
            statusPacket.distance++;
            verso=DESTRA;
        }

        //arduino_send_packet((Packet*)(&errorPacket));


        _delay_ms(500); // from delay.h, wait 1 sec
    }
}