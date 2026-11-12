/* @SymbolSize: 136 */
#include "types.h"

extern u8 *tt_scroll_wk;
extern u8 *tt_scale_buf;
extern s32 rcos(s32);

void tt_board_scale_step(void)
{
    u8 *s2;
    u8 *s0;
    s32 s1;

    s2 = tt_scroll_wk;
    s0 = tt_scale_buf;
    s1 = 0;

    do {
        s32 v0 = rcos(*(u16 *)(s2 + 30));
        s1 += 1;
        s0 += 16;
        *(s16 *)(s0 - 16) = (s16)v0;
        *(s16 *)(s0 - 14) = (s16)4096;
    } while (s1 != 25);

    *(u16 *)(s2 + 30) = *(u16 *)(s2 + 30) + 16;
}
