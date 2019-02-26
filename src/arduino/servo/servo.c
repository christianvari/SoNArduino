/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-21 14:29:20
 * @desc Library for SG90
 */

#include "servo.h"

void Wait(void)
{
   uint8_t i;
   for(i=0;i<50;i++)
   {
      _delay_loop_2(0);
      _delay_loop_2(0);
      _delay_loop_2(0);
   }

}

void initServo(){ 

/* Setto i registri di configurazione del timer 1 */


	TCCR1A=TCCRA_MASK;
	TCCR1B=TCCRB_MASK;


/* Setto il pin in output */

	DDRB |= PIN12_MASK;//mask;

/* Azzero il Timer 1 */

	TCNT1 = 0;

/* Setto il TOP della forma d'onda */

	ICR1 = TOP;
}

int main(void){
	initServo();
    while(1){

		OCR1B = POSITION_0;		/* Set servo shaft at 0° position */
		Wait();
		OCR1B = POSITION_90;	/* Set servo shaft at 90° position (20000us/4us)*/
		Wait();
		OCR1B = POSITION_N_90;	/* Set servo shaft at 90° position (20000us/4us)*/
		Wait();

		//_delay_ms(1500);
	}
}
