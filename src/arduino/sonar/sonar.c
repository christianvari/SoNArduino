/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-05 18:01:31
 * @modify date 2019-03-07 11:23:45
 * @desc Sonar Library
 */
 
#include "sonar.h"
 
volatile uint32_t overFlowCounter = 0;
 
void initSonar(){
    sei();
    TRIG_DDR |= (1<<TRIG_BIT);     // Set Trigger pin as output
    ECHO_DDR &= ~(1<<ECHO_BIT);      // Set Echo pin as input
}
 
/**
 *
 * Invia il segnale di trigger
 *
 */
void trigger_sonar(){
    TRIG_PORT &= ~(1<<TRIG_BIT);             // Abbasso il pin del trigger
    _delay_us(1);                       
    TRIG_PORT |=(1<<TRIG_BIT);               // Alzo il pin
    _delay_us(12);                           // Tengo alto per 10us (per sicurezza 12)
    TRIG_PORT &= ~(1<<TRIG_BIT);             // Abbasso il pin
    _delay_us(1);                            
}

ISR(TIMER3_OVF_vect){       // Timer1 overflow interrupt
    overFlowCounter++;      // Tengo il conto dei timer che ho riempito
    TCNT3=0;
}


uint16_t readSonar(){
    uint16_t dist_in_cm = 0;
    uint32_t trig_counter = 0;
    trigger_sonar();                    

    while(!(ECHO_PIN & (1<<ECHO_BIT))){     // Aspetto che il pin di echo venga alzato
        trig_counter++;
        if (trig_counter > SONAR_TIMEOUT){
            return TRIG_ERROR;
        }
    }
 
    TCNT3=0;                            // resetto il timer
    TCCR3B |= (1<<CS30);                // avvio il timer senza prescaler
    TIMSK3 |= (1<<TOIE3);               // attivo l'interrupt overflow sul timer 3
    overFlowCounter=0;                  // resetto il contatore di overflow
 
    while((ECHO_PIN & (1<<ECHO_BIT))){    // Aspetto che il pin venga abbassato
        if (((overFlowCounter*TIMER_MAX)+TCNT3) > SONAR_TIMEOUT){
            return ECHO_ERROR;            // L'oggetto è fuori dal range massimo
        }
    };
 
    TCCR3B = 0x00;                      // fermo il timer
    dist_in_cm = (((overFlowCounter*TIMER_MAX)+TCNT3)/(TO_CM*ISTRUZIONI_US));   // distance in cm
    return (dist_in_cm);
}

uint16_t getDistance(uint8_t precision){
    uint16_t distance = ECHO_ERROR;
    uint16_t tmp;
    uint8_t reads = 0;
    int i;
    for(i = 0; i< precision; i++){
        tmp = readSonar();
        if(tmp == TRIG_ERROR)
            return TRIG_ERROR;
        else if (tmp == ECHO_ERROR)
            continue;
        distance+= tmp;
        reads++;
    }
    distance=(distance - ECHO_ERROR)/reads;


}
