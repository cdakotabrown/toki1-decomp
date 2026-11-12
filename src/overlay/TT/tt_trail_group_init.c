#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;
extern u8 *tt_scroll_wk;

extern void trail_init(u8 *obj);

void tt_trail_group_init(void)
{
    u8 *v0;
    u8 *s0;
    u8 *t7;
    s32 s1;
    s32 s2;

    v0 = tt_player_wk;
    s0 = tt_trail_buf;
    t7 = tt_scroll_wk;

    *(u16 *)(t7 + 8) = 0x8CA0u;
    v0[83] = 1;
    v0[85] = 9;
    s1 = 0;
    s2 = 3;
    s0[146] = 9;

    do {
        trail_init(s0);
        s1 += 1;
        s0 += 164;
    } while (s1 != s2);
}
