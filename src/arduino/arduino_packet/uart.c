/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-08 12:16:07
 * @modify date 2019-03-08 12:16:07
 * @desc USART Library 
 */

#include "uart.h"


void setBaud57600(void) {
#define BAUD 57600
#include <util/setbaud.h>
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR0A |= (1<<U2X0);
#endif
#undef BAUD
}

void setBaud115200(void) {
#define BAUD 115200
#include <util/setbaud.h>
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR0A |= (1<<U2X0);
#endif
#undef BAUD
}

volatile uint8_t da_leggere = 0;
volatile uint8_t stato_receive;
volatile uint8_t stato_to_send;
volatile uint8_t data_to_send_length;
uint8_t data_received[4];
uint8_t data_to_send[10];


void UART_init(uint32_t baud) {

    switch(baud){
    case 57600: setBaud57600(); break;
    case 115200: setBaud115200(); break;
    default: return 0;
    }

    stato_receive=0;
    stato_to_send=0;
    data_to_send_length = 0;

    da_leggere = 0;


    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   /* Enable RX and TX */  
    sei();
}

uint8_t UART_da_leggere(){
    
    if(calcute_checksum(data_received, 3) == data_received[4]){
        da_leggere = 0;
        return 1;
    }
    
    da_leggere = 0;
    return -1;
}

//ritorna 1 se ha settatto il packet, 0 altrimenti
uint8_t arduino_receive_packet(CommandPacket* packet){


    if (!da_leggere) return 0;

    if(calcute_checksum(data_received, 3) != data_received[4]){
        da_leggere = 0;
        return 0;
    }

    packet->packet.type = data_received[2];
    packet->command = data_received[3];

    da_leggere = 0;
    return 1;
}


ISR(USART0_RX_vect) {

    if(da_leggere) return;
    uint8_t c=UDR0;

    switch (stato_receive)
    {
        case 0:
            if(c == HEADER0){
                data_received [0] = c;
                stato_receive++;
            }
            break;
        
        case 1:
            if(c == HEADER1){
                data_received [1] = c;
                stato_receive++;
            }
            else stato_receive = 0;
            break;
        
        case 2:
        case 3:
            data_received [stato_receive] = c;
            stato_receive++;
            break;
        case 4:
            
            data_received [4] = c;
            stato_receive=0;
            da_leggere = 1;
            break;
        
        default: 
            stato_receive=0;
            break;

    }
}

ISR(USART0_UDRE_vect){
    if (stato_to_send > data_to_send_length){
        stato_to_send = 0;
        UCSR0B &= ~_BV(UDRIE0);
    } else {
        UDR0 = data_to_send[stato_to_send];
        stato_to_send++;
    }
}


void arduino_send_packet(Packet* packet){

    data_to_send[0] = 0xaa;
    data_to_send[1] = 0x55; 
   

    data_to_send_length=2;
    int i;
    switch (packet->type)
    {
        case COMMAND:
            data_to_send[2]=(uint8_t) sizeof(CommandPacket);
            for(i=0; i<sizeof(CommandPacket);i++){
                data_to_send[3+i] = (*((uint8_t*)(packet+i)));
            }
            data_to_send_length += sizeof(CommandPacket)+1;
            break;
        case STATUS:
            data_to_send[2]=(uint8_t) sizeof(StatusPacket);
            for(i=0; i<sizeof(StatusPacket);i++){
                data_to_send[3+i] = (*((uint8_t*)(packet+i)));
            }
            data_to_send_length += sizeof(StatusPacket)+1;
            break;
        case ERROR:
            data_to_send[2]=(uint8_t) sizeof(ErrorPacket);
            for(i=0; i<sizeof(ErrorPacket);i++){
                data_to_send[3+i]= (*((uint8_t*)(packet+i)));
            }
            data_to_send_length += sizeof(ErrorPacket)+1;
            break;    
        default:
            break;
    }

    data_to_send[data_to_send_length] = calculate_checksum(data_to_send, data_to_send_length);

     UCSR0B |= _BV(UDRIE0);
}