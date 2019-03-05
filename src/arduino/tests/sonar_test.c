/**================================================== *
 * ==========  Includes  ========== *
 * ================================================== */
#pragma once
#include <avr/interrupt.h>
#include <stdlib.h>
#include "../arduino_packet/arduino_packet.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>

#define TRIGGER_PIN PC1 //PIN 36
#define ECHO_PIN PL0 //PIN 49 ICP4

static int pulse = 0;
static int i = 0;
int TimerOverflow = 0;

ISR(TIMER4_OVF_vect)
{
	TimerOverflow++;	//Aumento numero di overflow
}

void initSonar() {

    DDRC |= (1<<TRIGGER_PIN); //Setto pin in output
    DDRL &= ~(1<<ECHO_PIN); //Setto pin echo in input

    PORTL |= (1<<ECHO_PIN); //Setto pull up resistor in port C per il pin di echo

    sei(); //Attivo gestione interrupt

    TIMSK4 = (1 << TOIE4);	//Attivo interrupt overflow su timer4
    TCCR4A = 0; //uso il timer solo per contare
}

double fire(){

    long tempo_totale;
    double distance;
    /* Mando a 5V il TRIGGER_PIN per 10 us */
    PORTC |= (1<<TRIGGER_PIN);
    _delay_us(10);
    PORTC &= (1<<TRIGGER_PIN);
    
    TCNT4 = 0; //Azzero il timer
    TCCR4B = (1<<CS40) | (1<<ICES4); // No prescaler, Input Capture Edge Select

    TIFR4 = 1<<ICF4;	// Pulisco flag ICP4
	TIFR4 = 1<<TOV4;     // Pulisco flag Timer Overflow

    /* Calcolo lunghezza ECHO_PIN */

    while ((TIFR4 & (1 << ICF4)) == 0); //Aspetto che venga alzato il pin (rising edge)

	TCNT4 = 0;  //Azzero il timer
	TCCR4B = (1<<CS40); //Ora metto che catturo quando viene abbassato il pin (falling edge) ICES4 => 0

	TIFR4 = 1<<ICF4;	// Pulisco flag ICP4
	TIFR4 = 1<<TOV4;    // Pulisco flag Timer Overflow
	TimerOverflow = 0;  //Faccio ripartire il timer degli overflow

    while ((TIFR4 & (1 << ICF4)) == 0); //Aspetto che venga abbassato il pin (falling edge)

	tempo_totale = ICR4 + (65535 * TimerOverflow);	//Conto il tempo passato da quando è stato alzato il pin

    /**
     *
     * La frequenza della cpu è 16 Mhz e il timer non ha prescaler.
     * Il tempo di clock è 1/16Mhz = 6.25 * 10^-8
     * La velocità del suono a 20 gradi è 343,4 m/s => 34340 cm/s
     * 
     * La distanza posso calcola come => 
     * 
     * D = velocita_suono * tempo_passato / 2 * periodo_clock
     * = 34340 * tempo_totale / 2 * periodo_clock
     * = 17170 * tempo_totale * 6.25 * 10^-8
     * = tempo_totale / 931,857
     *
     */
    
    
	distance = (double) tempo_totale / 931.857;
    
    return distance;
    
}

int main(void)
{

    printf_init();
    initSonar();
    
    while(1){

        printf("distanza: %f\n", fire());

        _delay_ms(1000);
    }
    
}