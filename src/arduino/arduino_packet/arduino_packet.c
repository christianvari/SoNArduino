
#include "uart.h"
#include "arduino_packet.h"
#include <stdlib.h>


uint8_t calcute_checksum(uint8_t *buf, int n){
    return 0;
}


int arduino_send_packet(Packet* packet){
    uint8_t buf[30];
    UART_putChar(0xaa);
    
    buf[0]=0xaa;
    UART_putChar(0x55);
    buf[1]=0x55;    

    int n=2;
    int i;
    switch (packet->type)
    {
        case COMMAND:
            UART_putChar(sizeof(CommandPacket));
            buf[2]=(uint8_t) sizeof(CommandPacket);
            for(i=0; i<sizeof(CommandPacket);i++){
                UART_putChar(*((uint8_t*)(packet+i)));
                buf[3+i] = (*((uint8_t*)(packet+i)));
            }
            n += sizeof(CommandPacket)+1;
            break;
        case STATUS:
            UART_putChar(sizeof(StatusPacket));
            buf[2]=(uint8_t) sizeof(StatusPacket);
            for(i=0; i<sizeof(StatusPacket);i++){
                UART_putChar(*((uint8_t*)(packet+i)));
                buf[3+i] = (*((uint8_t*)(packet+i)));
            }
            n += sizeof(StatusPacket)+1;
            break;
        case ERROR:
            UART_putChar(sizeof(ErrorPacket));
            buf[2]=(uint8_t) sizeof(ErrorPacket);
            for(i=0; i<sizeof(ErrorPacket);i++){
                UART_putChar(*((uint8_t*)(packet+i)));
                buf[3+i]= (*((uint8_t*)(packet+i)));
            }
            n += sizeof(ErrorPacket)+1;
            break;    
        default:
            break;
    }

    UART_putChar(calcute_checksum(buf, n));

    return n+1;
}

Packet* arduino_receive_packet(Packet* packet){

    return NULL;
}
