/* tt_recs_init - TT overlay, Address: 0x80148DD4. */
#include "types.h"

extern u8 *tt_recs_buf;

void tt_recs_init(void)
{
    s32 i = 0;
    u8 *p = tt_recs_buf;
    for (; i < 8; i++) {
        p[0] = 0;
        *(s16 *)(p + 16) = 11;
        *(s16 *)(p + 62) = 10;
        *(s16 *)(p + 64) = 15499;
        p[66] = 240;
        p[67] = 208;
        p += 88;
    }
}
