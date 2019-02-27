#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "../arduino_packet/arduino_packet.h"

#define PIN_MASK 0xFF

static int pulse = 0;
static int i = 0;

ISR(INT0_vect)
{
    if (i==1)
    {
        TCCR1B &= (~(1<<CS10)) & (~(1<<CS11));
        pulse = TCNT1;
        TCNT1 = 0;
        i = 0;
    }
    if (i==0)
    {
        TCCR1B |= (1<<CS10) | (1<<CS11);
        i=1;
    }
}

int main(void)
{

    printf_init();
    DDRD |= (1<<PD0);
    
    DDRB &= ~(1<<PB0); //set PIN_MASK pins as input
    PORTB |= PIN_MASK; //enable pull up resistors

    PCICR |= (1 << PCIE0); // set interrupt on change, looking up PCMSK0
    PCMSK0 |= PIN_MASK; 
    
    _delay_ms(50);
    
    printf("parto\n");
    TCCR1A = 0;
    
    int16_t COUNTA = 0;    
    
    sei();
    
    while(1)
    {
        //trig
        PORTD |= (1<<PD0);
        _delay_us(15);
        PORTD &= ~(1<<PD0);

        COUNTA = pulse/10000;

		printf("COUNTA: %d, i: %d\n", COUNTA, i);

        _delay_ms(1000);
    }
}