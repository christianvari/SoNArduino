/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-27 10:42:28
 * @desc Library for SG90
 */

#include "servo.h"

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
	_delay_ms(500);
}

uint8_t setAngle(uint8_t angle){

	if(angle > 180 || angle < 0)
		return -1;
	
	OCR1B =(SG90_WIDTH/180 * angle + SG90_MIN_ANGLE)+0.5;

	return 0;

}

uint8_t getAngle(){

	return 180 / SG90_WIDTH * (OCR1B - SG90_MIN_ANGLE)+0.5; //Faccio prima la divisione per non andare in overflow
}

uint8_t modifyAngle(uint8_t type, uint8_t delta){

	if(delta < 0 || delta > 180)
		return -1;

	uint8_t angle = getAngle();
	switch (type)
	{
		case 0:
			if(angle - delta < 0)
				return -1;
			setAngle(angle-delta);
			break;
	
		case 1:
			if(angle + delta > 180)
				return -1;
			setAngle(angle+delta);
			break;
	
		default:
			return -1;
	}

	return 0;
}