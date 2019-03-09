#include <stdint.h>

//types
#define COMMAND 0
#define STATUS 1
#define ERROR 2
#define EVENT 3
#define CONFIGURATION 4
#define DISCONNECTION 5

//commands
#define START 0
#define STOP 1
#define SET_VELOCITY 2
#define SET_PRECISION 3
#define SEND_CONFIG 4

//events
#define STARTED 0
#define STOPPED 1
#define DISCONNECTED 2
#define SETTED_PRECISION 3
#define SETTED_VELOCITY 4

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
    CantHandleCommand = 0x0,
    SetOutOfRange = 0x1

} ;

typedef struct{
    Packet packet;
    uint8_t event;
}EventPacket;

typedef struct{
    Packet packet;
    uint8_t velocity;
    uint8_t precision;
}ConfigurationPacket;

uint8_t calculate_checksum(uint8_t *, int );