/**================================================== *
 * ==========  Includes  ========== *
 * ================================================== */
#pragma once
#include <stdlib.h>
/*
#include "../arduino_packet/arduino_packet.h"
#include <util/delay.h>
*/
#include <stdio.h>
#include <stdint.h>

/*
#include <avr/io.h>
#include <avr/iom2560.h>
*/
//#define TRIGGER_PIN PC4 //PIN 33
//#define ECHO_PIN PD6 //PIN 49 ICP4

#define SONARS_NUM 1

void Sonar_init(void);
uint8_t sonar_ping(void);
uint8_t sonar_get_last(void);