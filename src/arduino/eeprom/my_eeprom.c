/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-05 10:45:47
 * @modify date 2019-03-05 10:45:47
 * @desc EEPROM Library
 */

#include "my_eeprom.h"
#include "../arduino_packet/uart.h"

uint8_t save (uint8_t valore, uint8_t* tipo){

    if(tipo != ADDR_PRECISIONE && tipo != ADDR_VELOCITA) return -1;

    eeprom_busy_wait();

    eeprom_write_byte(tipo,valore);

    return 0;

}


uint8_t load (uint8_t* tipo){

    if(tipo != ADDR_PRECISIONE && tipo != ADDR_VELOCITA) return -1;

    eeprom_busy_wait();

    uint8_t valore = eeprom_read_byte(tipo);

    printf("Lettura %d\n", valore);

    if(valore == 255){
        
        if(tipo == ADDR_PRECISIONE) valore = PRECISIONE_DEFAULT;
        else valore = VELOCITA_DEFAULT;
        save(valore, tipo);
    }

    return valore;
}

