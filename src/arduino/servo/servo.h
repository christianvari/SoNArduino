/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-02-21 14:29:20
 * @modify date 2019-02-27 10:42:24
 * @desc Library for SG90
 */


/**
 *
 * Utilizzo il PIN PWM 12 che corrisponde a PB6 e usa l'output compare register OC1B
 * L'ATMEGA2650 va a 16Mhz
 *
 */



/**================================================== *
 * ==========  Includes  ========== *
 * ================================================== */
#pragma once
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>

/**================================================== *
 * ==========  Defines  ========== *
 * ================================================== */

/* Definisco la maschera per la porta B per avere il pin 12 in output */
#define PIN12_MASK (1<<PB6)

/* Fast PWM Mode 14 con TOP su ICR non invertito */
#define TCCRA_MASK (1<<COM1B1)|(1<<WGM11)

/* Prescaler settato a 64 e settati bit per mode 14 fast pwm*/
#define TCCRB_MASK (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10)

/* Definisco periodi PWM per il servo sg90 */
#define SG90_MAX_ANGLE 620
#define SG90_MIN_ANGLE 130
#define SG90_WIDTH 490.0f    //Ho bisogno che sia decimale per fare calcoli corretti in setAngle

/* Definisco TOP della forma d'onda pwm */
#define TOP 4999 // f_pwm=f_cpu/(prescale(1+top)) => top= (f_cpu / (f_pwm * prescale)) - 1

#define AUMENTA_ANGOLO 1
#define DIMINUISCI_ANGOLO 0



/**================================================== *
 * ==========  Functions  ========== *
 * ================================================== */


/* Inizializza i registri del microprocessore per utilizzare il servo */
void initServo(void);

/**
 *
 * Setta l'angolo passato come parametro
 * Ritorna  => 0 se è andato tutto ok
 * Ritorna  => 255 in caso di errore
 *
 */
uint8_t setAngle(uint8_t angle);

/**
 *
 * Ritorna l'angolo del servomotore
 *
 */
uint8_t getAngle(void);

/**
 *
 * Modifica l'angolo attuale del servomotore.
 * Parametri:   type => permette di scegliere il tipo di operazione
 *              type == 1 => aumenta l'angolo
 *              type == 0 => diminuisce l'angolo
 *              delta => angolo da aggiungere o sottrarre a quello attuale
 * 
 * Ritorna      => 0 se è andato tutto ok
 * Ritorna      => 255 in caso di errore
 */
uint8_t modifyAngle(uint8_t type, uint8_t delta);



