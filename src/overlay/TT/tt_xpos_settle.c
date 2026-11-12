/* @SymbolSize: 104 */
#include "types.h"

extern u8 *tt_boss_wk;

void tt_xpos_settle(void)
{
    u8 *p;
    s32 x;

    p = tt_boss_wk;
    x = *(s32 *)(p + 32);
    if (x < -0x200000) {
        *(s32 *)(p + 32) = *(s32 *)(p + 32) + 0x10000;
        return;
    }
    if (x > 0x200000) {
        *(s32 *)(p + 32) = *(s32 *)(p + 32) - 0x10000;
        return;
    }
    *(s16 *)(p + 6) = 2;
}
