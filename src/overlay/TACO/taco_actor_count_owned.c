/* taco_actor_count_owned - TACO overlay, Address: 0x8015CCA8, 412 bytes. */
#include "types.h"

extern u8 *taco_actor_rec;

s32 taco_actor_count_owned(s32 owner)
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 16; i < 63; i++) {
        if (taco_actor_rec[i * 136 + 0x85]) {
            if (!(taco_actor_rec[i * 136 + 1] & 0x80)) {
                taco_actor_rec[i * 136 + 0x85] = 0;
            }
        }
        if (owner != 255) {
            if (owner == taco_actor_rec[i * 136 + 0x86]
                && taco_actor_rec[i * 136 + 0x85]) {
                n++;
            }
        } else if (taco_actor_rec[i * 136 + 0x85]) {
            n++;
        }
    }
    return n;
}
