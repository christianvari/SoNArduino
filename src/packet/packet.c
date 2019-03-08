#include "packet.h"


uint8_t calculate_checksum(uint8_t *buf, int n){
    return 0;
}

uint16_t concatenateBytes(uint8_t byte1, uint8_t byte2){
    return ((uint16_t)byte2 << 8) | byte1;
}
