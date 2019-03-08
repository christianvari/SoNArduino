#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/iom2560.h>

#define F_CPU 16000000UL

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

volatile uint8_t command_received = -1;
volatile uint8_t command_to_send = -1;
volatile uint8_t da_leggere = 0;



void UART_init(uint32_t baud) {

  switch(baud){
  case 57600: setBaud57600(); break;
  case 115200: setBaud115200(); break;
  default: return 0;
  }
  

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
  UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   /* Enable RX and TX */  
  sei();
}


void UART_putChar(uint8_t c) {

    command_to_send = c;
    UCSR0B |= _BV(UDRIE0); // enable transmit interrupt
}

uint8_t UART_getChar(){
    
    if(da_leggere)
        return command_received;
    return -1;

}


ISR(USART0_RX_vect) {
    uint8_t command_received=UDR0;
    da_leggere = 1;
}

ISR(USART0_UDRE_vect){
  if (command_to_send == -1){
    UCSR0B &= ~_BV(UDRIE0);
  } else {
    UDR0 = command_to_send;
    command_to_send = -1;
}