/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-21 14:29:20
 * @desc Library for SG90
 */

#include "servo.h"

void startServo(){ 

	// we will use timer 1
	TCCR1A=TCCRA_MASK;
	TCCR1B=TCCRB_MASK;
	/* clear all higher bits of output compare for timer
	OCR1AH=0;
	OCR1BH=0;
	OCR1CH=0;
	OCR1CL=0;*/

/* Setto il pin in output */

	DDRB |= PIN12_MASK;//mask;

/* Azzero il Timer 1 */

	TCNT1 =0;

	ICR1=4999;

	while(1){
		// we write on the output compare register a value
		// that will be proportional to the opposite of the
		// duty_cycle
		//OCR1CL = 3750;	/* Set servo shaft at 0° position (15000us/4us)*/
		OCR1BL = 65;	/* Set servo shaft at 0° position (15000us/4us)*/
		_delay_ms(1500);
		OCR1BL = 175;	/* Set servo shaft at 90° position (20000us/4us)*/
		_delay_ms(1500);
	}
}

int main(void){
    startServo();
}
