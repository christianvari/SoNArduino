/**
 * @author Christian Vari
 * @email vari.christian@gmail.com
 * @create date 2019-03-08 12:16:07
 * @modify date 2019-03-08 12:16:07
 * @desc USART Library 
 */

#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/iom2560.h>
#include "../../packet/packet.h"

#define F_CPU 16000000UL
#define HEADER0 0xaa
#define HEADER1 0x55

void UART_init(uint32_t baud);
uint8_t UART_da_leggere(void);
uint8_t arduino_receive_packet(CommandPacket* packet);
void arduino_create_packet(Packet* packet);