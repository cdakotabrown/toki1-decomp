/* @CompileFlags: -cckr */
/* @RodataBase: 0x8014D750 */
#include "types.h"

extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_step_wk;
extern u8 *tt_player_wk;
extern u8 *tt_text_buf;

void tt_sprite_anim()
{
    u8 *p;
    u8 *q;
    u8 *r;
    u8 *s;
    u8 *u;
    int i;

    r = tt_step_wk;
    p = tt_scroll_wk;
    s = tt_player_wk;
    q = tt_anim_buf + 4000;
    u = tt_text_buf;
    if (*(u16 *)(p + 16) >= 44) {
        *(u16 *)(p + 16) = 0;
        *(u16 *)(r + 2) += 1;
    }
    switch (*(u16 *)(p + 16)) {
    case 8:
        s[62] = 8;
        break;
    case 16:
        s[62] = 9;
        break;
    case 22:
        s[62] = 10;
        break;
    case 28:
        s[62] = 11;
        break;
    case 34:
        s[62] = 12;
        break;
    case 40:
        s[62] = 13;
        break;
    case 42:
        s[62] = 14;
        break;
    case 44:
        s[62] = 15;
        break;
    }
    for (i = 0; i < 4; i++) {
        *(s32 *)(q + 36) += -0x40000;
        *(s16 *)(q + 22) = *(s16 *)(q + 38);
        q += 100;
    }
    for (i = 0; i < 7; i++) {
        *(s16 *)(u + 12) += 4;
        u += 20;
    }
    tt_text_buf[120] = 0;
    *(s16 *)(tt_anim_buf + 22) += 4;
    *(u16 *)(p + 16) += 1;
}
