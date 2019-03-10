#include <stdio.h>
#include "../../packet/packet.h"


int main(){

    uint8_t start[8] = {0xaa, 0x55, 0, 0, 0};

    uint8_t stop[8] = {0xaa, 0x55, 0, 1, 0};

    uint8_t vel[8] = {0xaa, 0x55, 0, 2, 3};

    uint8_t prec[8] = {0xaa, 0x55, 0, 3, 3};

    printf("cs for start is %x\n", calculate_checksum(start, 5));
    printf("cs for stop is %x\n", calculate_checksum(stop, 5));

    printf("cs for vel is %x\n", calculate_checksum(vel, 5));

    printf("cs for prec is %x\n", calculate_checksum(prec, 5));

    return 0;

}