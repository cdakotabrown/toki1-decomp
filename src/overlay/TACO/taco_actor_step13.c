/* @SymbolSize: 356 */
/* Address: 0x80157EB8 */
/* Size: 356 bytes (89 instructions) */
/* (contiguous up to the next existing symbol 0x8015801C; @SymbolSize: 356 */
#include "types.h"

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))
#define ACB(n) (*(u8 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step13(s32 a0)
{
    s32 d;

    d = 10;
    switch (AC(124)) {
    case 0:
        AC(116) = (*(s16 *)(taco_actor_rec + 116) - AC(116)) / d + AC(116);
        AC(118) = (*(s16 *)(taco_actor_rec + 118) - AC(118)) / d + AC(118);
        AC(120) = -10050;
        if (AC(122) >= 61) {
            AC(124) = AC(124) + 1;
            AC(122) = 0;
        }
        break;
    case 1:
        if (AC(122) >= 61) {
            AC(124) = AC(124) + 1;
        }
        break;
    case 2:
        ACB(2) = 254;
        break;
    }
}
