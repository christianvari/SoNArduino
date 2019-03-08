#include <stdint.h>

//types
#define COMMAND 0
#define STATUS 1
#define ERROR 2

//commands
#define START 0
#define STOP 1
#define SET_VELOCITY 2
#define SET_PRECISION 3


typedef struct{
    uint8_t type;
}Packet;

typedef struct {
    Packet packet;
    uint8_t command;
    uint8_t payload;
}CommandPacket;

typedef struct{
    Packet packet;
    uint8_t angle;
    uint8_t distance;
}StatusPacket;

typedef struct{
    Packet packet;
    uint8_t error_code;
}ErrorPacket;

enum Error_Code{
  OutRange=0x0,
  UnknownCommand=0x1
} ;


uint8_t calculate_checksum(uint8_t *, int );