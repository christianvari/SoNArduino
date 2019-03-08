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