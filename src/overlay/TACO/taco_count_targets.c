/* Address: 0x80140548 */
/* Size: 412 bytes */
/* @SymbolSize: 412 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
#include "types.h"

extern u8 *taco_actor_rec;

s32 taco_count_targets(s32 *out)
{
    s32 n;
    s32 idx;

    n = 0;
    *out = -1;
    for (idx = 16; idx < 64; idx++) {
        if ((taco_actor_rec[idx * 136 + 1] & 0x80) && (taco_actor_rec[idx * 136 + 135] & 1)
            && taco_actor_rec[idx * 136 + 134] != 14) {
            n++;
            if (*out == -1) {
                *out = idx;
            }
        }
    }
    return n;
}
