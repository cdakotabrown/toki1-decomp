/* tt_recs_clear - TT overlay, Address: 0x80148DA0. */
#include "types.h"

extern u8 *tt_recs_buf;

void tt_recs_clear(void)
{
    s32 i = 0;
    u8 *p = tt_recs_buf;
    for (; i < 8; i++) {
        *p = 0;
        p += 88;
    }
}
