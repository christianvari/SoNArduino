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

#define BAUD 57600
#include <util/setbaud.h>

void UART_init(void);
uint8_t arduino_receive_packet(CommandPacket* packet);
void arduino_send_packet(Packet* packet);