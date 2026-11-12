/* Address: 0x80147AFC */
/* Size: 372 bytes (93 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

s32 get_taco_percent(s32 n, u8 *w)
{
    s32 total;
    s32 left;
    s32 i;

    total = 0;
    left = 0;
    for (i = 0; i < n; i++) {
        left += taco_actor_rec[i * 136 + 2309];
        total += w[i * 2] * *(s16 *)(taco_actor_rec + i * 136 + 2298);
    }
    return 100 - left * 100 / total;
}
