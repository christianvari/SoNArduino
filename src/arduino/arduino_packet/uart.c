/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-08 12:16:07
 * @modify date 2019-03-08 18:45:10
 * @desc USART Library 
 */

#include "uart.h"

volatile uint8_t da_leggere;
volatile uint8_t stato_receive;
volatile uint8_t stato_to_send;
volatile uint8_t data_to_send_length;
uint8_t data_received[6];
uint8_t data_to_send[10];


void UART_init() {

    //Setto il baudrate a 57600
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0A |= (1<<U2X0);


    stato_receive=0;
    stato_to_send=0;
    data_to_send_length = 0;

    da_leggere = 0;


    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // Dati a 8 bit
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   // Abilito TX e RX su UART
    sei();
}


//ritorna 1 se ha settatto il packet, 0 altrimenti
uint8_t arduino_receive_packet(CommandPacket* packet){

    if (!da_leggere) return 0;

    if(calculate_checksum(data_received, 5) != data_received[5]){
        da_leggere = 0;
        return 0;
    }

    packet->packet.type = data_received[2];
    packet->command = data_received[3];
    packet->payload = data_received[4];

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
        case 4:
            data_received [stato_receive] = c;
            stato_receive++;
            break;
        case 5:
            
            data_received [5] = c;
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
        UCSR0B &= ~_BV(UDRIE0);             //Disattiva interrupt per USART0_UDRE
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
        case EVENT:
            data_to_send[2]=(uint8_t) sizeof(EventPacket);
            for(i=0; i<sizeof(EventPacket);i++){
                data_to_send[3+i]= (*((uint8_t*)(packet+i)));
            }
            data_to_send_length += sizeof(EventPacket)+1;
            break;  
        case CONFIGURATION:
            data_to_send[2]=(uint8_t) sizeof(ConfigurationPacket);
            for(i=0; i<sizeof(ConfigurationPacket);i++){
                data_to_send[3+i]= (*((uint8_t*)(packet+i)));
            }
            data_to_send_length += sizeof(ConfigurationPacket)+1;
            break;  
        default:
            break;
    }

    data_to_send[data_to_send_length] = calculate_checksum(data_to_send, data_to_send_length);

    UCSR0B |= _BV(UDRIE0); // Attivo l'interrupt per inviare
}