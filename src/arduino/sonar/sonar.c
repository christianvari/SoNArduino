#include "sonar.h"

#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>



uint16_t counter;

// bit 0 of port C => PC0 => pin 37
uint8_t triggers = 0x1;   

// raised by isr when a falling edge of the echo is received
// bit 0 of port K => PK0 => pin A8
uint8_t echos = 0x1;


/* =======  FUNCTIONS  ======= */


/* =======  Counter  ======= */

void Counter_init(void){
  // set normal mode of operation
  TCCR4A=0;
  TCCR4B=(1 << CS42);
  OCR4A=0xFFFF;
}


/* =======  Sonar  ======= */

void Sonar_init() {
  Counter_init();
  
  DDRC=triggers;
  // level low
  PORTC=0x0;

  DDRK=0x0;
  // enable pull_up resistors
  PORTK=triggers;
    
  // mask all echoes
  PCMSK2 = 0;

  // enable interrupts
  PCICR|=(1<<PCIE2);

}


uint8_t sonar_ping(void){
  // if a reading is in progress return a failure
  /* echos=PINC; */
  /* if(echos) */
  /*   return 0; */

  // clear timers
  
  counter = 0;
  
  // send a trigger and wait 10 us
  PORTC=triggers;
  _delay_us(10);
  PORTC=0;

  TCNT4 = 0;

  sei();
  // enable interrupt on change for selected sonars
  PCMSK2=1<<PCINT16;
  cli();
  return 1;
}

uint8_t sonar_get_last(void){
  return counter;
}

ISR(PCINT2_vect) {
  uint16_t current_time=TCNT4;
  uint8_t current_echo=PINK;

  cli();
  printf("interrupt");

  // look only at the current bit
  uint8_t mask=0x1;

  // if nothing happened on that sonar, we skip
  if ((current_echo&mask)==(echos&mask))
    return;

  // in case of a rising  edge, we save the timer
  if (current_echo&mask) {
    counter = current_time;
  } else {
  // otherwise we put in the counters[s] the duration (echo end)
    counter = current_time-counter;
  }
  
  echos=current_echo;
  sei();
}