/**
 * @author Marco Pennese
 * @email marco.pennese.97@gmail.com
 * @create date 2019-03-05 10:58:31
 * @modify date 2019-03-05 10:58:31
 * @desc [description]
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "../arduino_packet/arduino_packet.h"
#include "../sonar/sonar.h"

#define PIN_MASK 0xFF

int main(void)
{

    printf_init();
    Sonar_init();

    printf("parto\n");
    _delay_ms(50);
    
    while(1)
    {

        sonar_ping();
		printf("timer: %d\n", sonar_get_last());

        _delay_ms(1000);
    }
}