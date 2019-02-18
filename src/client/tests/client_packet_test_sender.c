#include "../client_packet/client_packet.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    Packet packet;
    packet.type = COMMAND;

    CommandPacket commandPacket;
    commandPacket.packet = packet;
    commandPacket.command = 1;

    int pipefd[2];

    int fd = open("fifo", O_WRONLY);

    client_send_packet((Packet*)(&commandPacket), fd);

    char buf[2]={0xaa, 0x44};
    int ret = write(fd, buf, 2);
    if (ret == -1) perror("Error writing");

    packet.type = ERROR;
    ErrorPacket errorPacket;
    errorPacket.packet=packet;
    errorPacket.error_code = UnknownCommand;

    client_send_packet((Packet*)(&errorPacket), fd);

    packet.type = STATUS;
    StatusPacket statusPacket;
    statusPacket.packet=packet;
    statusPacket.angle=32;
    statusPacket.distance=52.0;

    client_send_packet((Packet*)(&statusPacket), fd);

    sleep(20);
}