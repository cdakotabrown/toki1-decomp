/* tt_view_reset - TT overlay, Address: 0x80149B10, Size: 144 bytes (36 insns) */
#include "types.h"

extern u8 *tt_scale_buf;
extern u8 *tt_anim_buf;

void tt_view_reset(void)
{
    u8 *a;
    u8 *p;

    a = tt_scale_buf;
    *(s16 *)(a + 20) = 0;
    a += 16;
    *(s16 *)(a + 2) = 4096;
    *(s16 *)(a + 18) = 4096;
    *(s16 *)(a + 20) = 0;
    *(s16 *)(a + 0) = *(s16 *)(a + 2);
    *(s16 *)(a + 16) = *(s16 *)(a + 18);

    p = tt_anim_buf + 4000;
    *(s32 *)(p + 32) = 0xFF940000;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    p[79] = 0;
    p += 100;
    *(s32 *)(p + 32) = 0xFFE80000;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    p[79] = 0;
    p += 100;
    *(s32 *)(p + 32) = 0xFFA00000;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    p[79] = 0;
    p += 100;
    *(s32 *)(p + 32) = 0x00200000;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    p[79] = 0;
}
