/* tt_rec_clear - TT overlay, Address: 0x80146BD0, Size: 52 bytes (13 insns) */
/* @PostProc: swap_registers v0:v1 --bidirectional */
#include "types.h"

extern u8 *tt_rec_pool;

void tt_rec_clear(void)
{
    u8 *p;
    s32 i;

    p = tt_rec_pool;
    i = 0;
    do {
        i += 4;
        p[88] = 0;
        p[176] = 0;
        p[264] = 0;
        p += 352;
        p[-352] = 0;
    } while (i != 8);
}
