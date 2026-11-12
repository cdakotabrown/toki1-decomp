/* @PostProc: swap_branch_operands --offsets 0x50 */
/* sparkle - TT overlay, Address: 0x80134ADC, Size: 224 bytes (56 insns) */
#include "types.h"

extern u8 *tt_boss_wk;

void sparkle(u8 *p)
{
    u8 *g;
    u16 s;

    g = tt_boss_wk;
    if (*(u16 *)(p + 88) == 15) {
        *(s16 *)(p + 98) = 0;
        *(s16 *)(p + 4) = 64;
        *(s16 *)(p + 10) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[15] = 0;
        p[14] = 0;
        p[13] = 0;
    }

    s = *(u16 *)(p + 4);
    switch (s) {
    case 32:
        *(s32 *)(p + 32) = ((*(s16 *)(g + 20) - *(u16 *)(p + 96)) << 16) +
                           *(s32 *)(p + 32);
        *(s16 *)(p + 96) = *(s16 *)(g + 20);
        break;
    case 64:
        *(u16 *)(p + 98) += 4;
        if ((u32)*(u16 *)(p + 98) >= 32) {
            *(s16 *)(p + 98) = 32;
            *(s32 *)(p + 44) = 0x00040000;
            *(s16 *)(p + 4) = 80;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        break;
    }
    *(s16 *)(p + 88) = *(u16 *)(p + 88) + 1;
}
