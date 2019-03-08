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
    uint16_t distance;
}StatusPacket;

typedef struct{
    Packet packet;
    uint8_t error_code;
}ErrorPacket;

enum Error_Code{
    CantHandleCommand = 0x0,
    SetOutOfRange = 0x1,
    TriggerError = 0x2

} ;


uint8_t calculate_checksum(uint8_t *, int );

uint16_t concatenateBytes(uint8_t byte1, uint8_t byte2);