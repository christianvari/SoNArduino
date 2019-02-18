
//types
#define COMMAND 0
#define STATUS 1
#define ERROR 2

//commands
#define START 0
#define STOP 1
#define SET 2


typedef struct{
    int type;
}Packet;

typedef struct {
    Packet packet;
    int command;
}CommandPacket;

typedef struct{
    Packet packet;
    float distance;
    int angle;
}StatusPacket;

typedef struct{
    Packet packet;
    int error_code;
}ErrorPacket;

enum Error_Code{
  OutRange=0x0,
  UnknownCommand=0x1
} ;
