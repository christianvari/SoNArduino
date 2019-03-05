#include "sonar.h"
volatile unsigned char triggerSent;
volatile unsigned int risingEdge;
volatile unsigned int fallingEdge;
volatile unsigned int distance;

unsigned int stato=0;
inline unsigned int getDistance(){
  stato++;
  return stato;
}

ISR (TIMER4_CAPT_vect)
{
	printf("Iterrupt ICR\n");
  if (TCCR4B == ((1<<ICES4) | (1<<CS40) | (1<<CS41))) // On rising edge
  {
    TCCR4B &= ~(1<<ICES4); // Next time detect falling edge
    risingEdge = ICR4; // Save current count
  }
  else // On falling edge
  {
    TCCR4B |= (1<<ICES4); // Next time detect rising edge
    fallingEdge = ICR4; // Save current count

    /**
     * 
     * 16 000 000 / 64 = 250000 => 4 us.
     * 4us * (MAX_TIMER * n_overflow + ICR4) / 2 * 34.3 cm/us
     * (2 * 65536 *novf)* 0.0343 =
     * 2*ICR4 /1000000*343
     *
     */
    
    
    distance = getDistance();
	  TCNT4 = 0;
  }
}

void initSonar() {


    DDRC |= (1<<TRIGGER_PIN); //Setto pin in output
    DDRD &= ~(1<<ECHO_PIN); //Setto pin echo in input

    //Setto Interrupt Capture Noise Canceller, e prescaler a 64 sul timer 4

    TIFR4 |= 1<<ICF4;	

    TCCR4B |= (1 << CS41) | (1 << CS40); 

/**
 *
    Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
    When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally enabled), the
    Timer/Countern Input Capture interrupt is enabled. The corresponding Interrupt Vector (see “Interrupts” on page
    101) is executed when the ICFn Flag, located in TIFRn, is set.

 *
 */
     
    TIMSK4 |= (1 << ICIE4);

/**
 * This bit selects which edge on the Input Capture pin (ICP1) that is used to trigger a capture event. When
    the ICES1 bit is written to zero, a falling (negative) edge is used as trigger, and when the ICES1 bit is
    written to '1', a rising (positive) edge will trigger the capture.
    When a capture is triggered according to the ICES1 setting, the counter value is copied into the Input
    Capture Register (ICR1). The event will also set the Input Capture Flag (ICF1), and this can be used to
    cause an Input Capture Interrupt, if this interrupt is enabled.
    When the ICR1 is used as TOP value (see description of the WGM1[3:0] bits located in the TCCR1A and
    the TCCR1B Register), the ICP1 is disconnected and consequently the Input Capture function is
    disabled.
 *
 */

    TCCR4B |= 1 << ICES4; // Attivo  l'interrupt capture pin sul rising edge

    sei();

    printf("Inizializzato\n");
}

int main()
{

    printf_init();
    initSonar();
    
    while(1){

      printf("Start sonar\n");
      PORTC |= (1<<TRIGGER_PIN);
      _delay_us(10);
      PORTC &= (1<<TRIGGER_PIN);
      TCNT4 = 0;

      _delay_ms(1000);
      printf("distanza: %d\n", distance);
    }
    
}