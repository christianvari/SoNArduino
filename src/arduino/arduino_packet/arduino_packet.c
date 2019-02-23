
#include "uart.h"
#include "arduino_packet.h"
#include <stdlib.h>
#include <stdio.h>



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

void arduino_receive_packet(CommandPacket* packet){
 
    //printf("In attesa\n");
    uint8_t buf[30];
    
    S:
    //printf("stato0\n");
    buf[0]=0;
    while(buf[0] != 0xaa){
        buf[0]=UART_getChar();
        //printf("received byte: %x\n", buf[0]);
    }
    //printf("stato1\n");
    buf[1]=UART_getChar();
    //printf("received byte: %x\n", buf[1]);
    if(buf[1]!=0x55){
        goto S;
    }
    //printf("stato2\n");
    uint8_t n = UART_getChar();

    buf[2]= n;
    //printf("received byte: %x\n", buf[2]);
    //printf("stato3\n");
    int i;
    int j;
    
    //printf("waiting for packet byte\n");
    packet->packet.type = UART_getChar();
    buf[3] = packet->packet.type;
    //printf("received byte: %x\n", buf[3]);
    //printf("stato4\n");
    
    
    //printf("waiting for packet byte\n");
    packet->command = UART_getChar();
    buf[4] = packet->command;
    //printf("received byte: %x\n", buf[4]);
    //printf("stato5\n");
    

    uint8_t cs = UART_getChar();
    //printf("received byte: %x\n", cs);
    if(calcute_checksum(buf, 5) != cs) goto S;



}
