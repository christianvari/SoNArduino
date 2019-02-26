/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-21 14:29:20
 * @desc Library for SG90
 */

/**
 *
 * Utilizzo il PIN PWM 12 che corrisponde a PB6 e usa l'output compare register OC1B
 * L'ATMEGA2650 va a 16Mhz
 *
 */


/*--------  Includes  --------*/

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>


/*--------  Constants  --------*/

/* Definisco la maschera per la porta B per avere il pin 12 in output */
#define PIN12_MASK (1<<PB6)

/* Fast PWM non invertito */
#define TCCRA_MASK (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11)

/* Prescaler settato a 64 */
#define TCCRB_MASK (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10)

/* Definisco periodi PWM per il servo sg90 */

#define POSITION_0 (uint8_t) 3750; /* 15000us(specifica del servomotore)/4us(periodo) */
#define POSITION_90 (uint8_t) 5000; /* 20000us (specifica del servomotore)/4us(periodo) */
#define POSITION_N_90 (uint8_t) 2500; /* 10000us (specifica del servomotore)/4us(periodo) */

/* Definisco TOP della forma d'onda pwm */

#define TOP 4999; /* f_pwm=f_cpu/(prescale(1+top)) => top= (f_cpu / (f_pwm * prescale)) - 1 */


/*--------  Functions  --------*/

void initServo(void);


