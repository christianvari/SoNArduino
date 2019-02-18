#pragma once
#include "../../packet/packet.h"


int client_send_packet(Packet* , int);
Packet* client_receive_packet(Packet*, int);
void client_print_packet(Packet*);
