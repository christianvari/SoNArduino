/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-05 10:45:47
 * @modify date 2019-03-05 10:45:47
 * @desc EEPROM Library
 */

#include "my_eeprom.h"

uint8_t save (uint8_t valore, uint8_t* tipo){

    if(tipo != ADDR_PRECISION && tipo != ADDR_VELOCITY) return 1;
    if(tipo == ADDR_PRECISION && valore > MAX_PRECISION) return 1;
    if(tipo == ADDR_VELOCITY && valore > MAX_VELOCITY) return 1;

    eeprom_busy_wait();

    eeprom_write_byte(tipo,valore);

    return 0;

}


uint8_t load (uint8_t* tipo){

    if(tipo != ADDR_PRECISION && tipo != ADDR_VELOCITY) return 1;

    eeprom_busy_wait();

    uint8_t valore = eeprom_read_byte(tipo);

    if(valore == 255){
        
        if(tipo == ADDR_PRECISION) valore = DEFAULT_PRECISION;
        else valore = DEFAULT_VELOCITY;
        save(valore, tipo);
    }

    return valore;
}

