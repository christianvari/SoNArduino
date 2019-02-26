/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-21 14:29:20
 * @desc Library for SG90
 */

#include "servo.h"
#include "uart.h"

void Wait(void)
{
   uint8_t i;
   for(i=0;i<25;i++)
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

	/* Posiziono il servo nello stato di partenza */
	OCR1BH = 0;
	OCR1BL = 0;
	OCR1B = SG90_MIN_ANGLE;
}

uint8_t setAngle(uint8_t angle){

	if(angle > 180 || angle < 0)
		return -1;
	
	OCR1B =(SG90_WIDTH/180 * angle + SG90_MIN_ANGLE);

	return 0;

}

uint16_t getAngle(){

	return OCR1B;
}
