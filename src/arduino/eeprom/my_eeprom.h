/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-05 10:43:35
 * @modify date 2019-03-05 10:43:35
 * @desc EEPROM Library
 */


/**================================================== *
 * ==========  Includes  ========== *
 * ================================================== */

#pragma once
#include <avr/eeprom.h>

/**================================================== *
 * ==========  Defines  ========== *
 * ================================================== */

#define ADDR_VELOCITA (uint8_t*) 0x00  // Definisce step angolo
#define ADDR_PRECISIONE (uint8_t*) 0x01 // Definisco numero di misurazioni per posizione

//Valori default
#define VELOCITA_DEFAULT 1
#define PRECISIONE_DEFAULT 1


/**================================================== *
 * ==========  Functions  ========== *
 * ================================================== */

/**
 *
 * Scrive valore nella locazione designata da tipo (che può essere ADDR_VELOCITA o ADDR_PRECISIONE)
 * Ritorna  => 0 se è andato tutto ok
 * Ritorna  => 255 in caso di errore
 *
 */

uint8_t save (uint8_t valore, uint8_t* tipo);

/**
 *
 * legge valore nella locazione designata da tipo (che può essere ADDR_VELOCITA o ADDR_PRECISIONE)
 * Ritorna  => valore se è andato tutto ok
 * Ritorna  => 255 in caso di errore
 *
 */

uint8_t load (uint8_t* tipo);