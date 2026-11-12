/* @SymbolSize: 468 */
/* Address: 0x80157CE4 */
/* Size: 468 bytes (117 instructions) */
/* (@SymbolSize: 468 windows it correctly meanwhile).  No duplicate rows for */
#include "types.h"

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))
#define ACB(n) (*(u8 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step14(s32 a0)
{
    s32 d;

    d = 10;
    switch (AC(124)) {
    case 0:
        AC(116) = (*(s16 *)(taco_actor_rec + 116) - AC(116)) / d + AC(116);
        AC(118) = (*(s16 *)(taco_actor_rec + 118) - AC(118)) / d + AC(118);
        AC(120) = -10000;
        if (AC(122) >= 61) {
            AC(124) = AC(124) + 1;
            AC(122) = 0;
        }
        break;
    case 1:
        if (AC(122) >= 91) {
            AC(124) = AC(124) + 1;
            AC(122) = 0;
        }
        break;
    case 2:
        AC(102) = 2048 - AC(122) * 64;
        if (AC(102) <= 0) {
            AC(124) = AC(124) + 1;
            AC(122) = 0;
        }
        break;
    case 3:
        ACB(2) = 253;
        break;
    }
}
