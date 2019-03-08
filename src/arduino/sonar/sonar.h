/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-05 18:01:31
 * @modify date 2019-03-07 11:23:35
 * @desc Sonar Library
 */


#pragma once
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/iom2560.h>

/* Trigger su scheda */

#define TRIG_DDR    DDRD
#define TRIG_PORT   PORTD
#define TRIG_PIN    PIND
#define TRIG_BIT    PD0             //Primo pin digital CL

/* Echo su scheda */

#define ECHO_DDR    DDRE
#define ECHO_PORT   PORTE
#define ECHO_PIN    PINE
#define ECHO_BIT    PE4             // Pin digitale 2

#define VELOCITA_SUONO  343         // Velocità del suono a 20 gradi centigradi
#define MAX_SONAR_RANGE 2          // Massimo range del hc-sr04 in metri
#define ATTESA_SICUREZZA_TEST 500   // Tempo di attesa prima di inviare nuovo trigger
#define TIMER_MAX 65535             // 65535 per timer a 16 bit


#define SONAR_OUT_OF_RANGE -1

#define ISTRUZIONI_US (F_CPU/1000000)   // istruzioni per microsecondo
#define ISTRUZIONI_MS (F_CPU/1000)      // istruzioni per millisecondo


#define SONAR_TIMEOUT ((F_CPU*MAX_SONAR_RANGE)/VELOCITA_SUONO) //Tempo massimo attesa echo
#define SONAR_DELAY 50
#define MIN_READS 2
#define TO_CM 58              //((10000*2)/VELOCITA_SUONO)

/**
 *
 * Inizializza i pin del sonar e abilita gli interrupt globali
 *
 */

void initSonar(void);

/**
 *
 * Calcola la distanza rilevata
 * Presuppone che sia stata eseguita prima initSonar()
 * Ritorna  => distanza se tutto ok
 * Ritorna  => SONAR_OUT_OF_RANGE se non c'è segnale di ritorno
 *
 */
uint8_t readSonar(void);

/**
 *
 * Calcola la distanza media rilevata
 * Presuppone che sia stata eseguita prima initSonar()
 * Ritorna  => distanza se tutto ok
 * Ritorna  => SONAR_OUT_OF_RANGE se non c'è segnale di ritorno
 *
 */
uint8_t getDistance(uint8_t precision);
