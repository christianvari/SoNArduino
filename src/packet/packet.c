#include "packet.h"


uint8_t calculate_checksum(uint8_t *buf, int n){
      
      uint8_t cs=0;
      int i;

      for (i = 0; i < n; ++i)
            cs += (unsigned int)(*buf++);
      return cs;
}