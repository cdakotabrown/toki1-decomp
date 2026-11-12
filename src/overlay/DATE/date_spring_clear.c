/* Function: date_spring_clear */
/* Overlay: DATE */
/* Address: 0x80140714, size 72 bytes (18 instructions) */
#include "common.h"

extern u8 class_w[][68];

void date_spring_clear() {
    short i;

    for (i = 0; i < 20; i++) {
        class_w[i][6667] &= 0x7f;
    }
}
