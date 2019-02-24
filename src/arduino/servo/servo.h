/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-21 14:29:20
 * @desc Library for SG90
 */

/**
 *
 * Utilizzo il PIN PWM 13 che corrisponde a PB7 e ha il timer OC1A
 * L'ATMEGA2650 va a 16Mhz
 *
 */


/*--------  Includes  --------*/

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

/*--------  Constants  --------*/

/* Definisco la maschera per la porta B per avere il pin 12 in output */
#define PIN12_MASK (1<<PB6)

/* Fast PWM non invertito */
#define TCCRA_MASK (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11)

/* Prescaler settato a 64 */
#define TCCRB_MASK (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10)

/*--------  Functions  --------*/

void startServo();


