/* taco_actor_count_alive - TACO overlay, Address: 0x8015CE44, 200 bytes. */
#include "types.h"

extern u8 *taco_actor_rec;

s32 taco_actor_count_alive(void)
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 16; i < 63; i++) {
        if (taco_actor_rec[i * 136 + 1] & 0x80) {
            n++;
        }
    }
    return n;
}
