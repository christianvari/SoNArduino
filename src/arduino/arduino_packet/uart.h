/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-08 12:16:07
 * @modify date 2019-03-08 18:45:06
 * @desc USART Library 
 */

/**================================================== *
 * ==========  Includes  ========== *
 * ================================================== */

#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/iom2560.h>
#include "../../packet/packet.h"

/**================================================== *
 * ==========  Defines  ========== *
 * ================================================== */


#define F_CPU 16000000UL
#define HEADER0 0xaa
#define HEADER1 0x55

/* Setto il baudrate ed eseguo la macro in util/setbaud.h */
#define BAUD 57600
#include <util/setbaud.h>


/**================================================== *
 * ==========  Functions  ========== *
 * ================================================== */


/* Inizializzo la UART */

void UART_init(void);

/**
 *
 * Fa il parsing dei byte ricevuti sulla UART in un CommandPacket
 * Ritorna  => 1 se il pacchetto è arrivato
 * Ritorna  => 0 se non è arrivato il pacchetto
 *
 */
uint8_t arduino_receive_packet(CommandPacket* packet);

/**
 *
 * Invia il pacchetto di massimo 10 byte sulla UART
 * 
 * FORMATO => aa 55 sizeof(tipo_pacchetto) (dati) cs 
 *
 */
void arduino_send_packet(Packet* packet);