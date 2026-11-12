/* tt_pair_disp - TT overlay, Address: 0x80144844, Size: 432 bytes (108 insns) */
#include "types.h"

extern void anim_sprite_disp(u8 *dst, u8 *src, u16 mode);

extern u8 *tt_game_wk;
extern u8 *tt_player_wk;
extern u8 *tt_cel_buf;

void tt_pair_disp(void)
{
    u8 *a;
    u8 *b;
    u8 *q;
    u8 *p;
    u32 v;
    u32 sel;

    a = tt_player_wk;
    b = tt_cel_buf;
    if (a[0] == 0) {
        return;
    }

    if (*(u16 *)(tt_player_wk + 80) & 2) {
        if (*(u16 *)(tt_game_wk + 8) & 1) {
            return;
        }
    }

    q = tt_player_wk;
    v = *(u16 *)(q + 4);
    if (v >= 96 && v < 100) {
        if ((q[64] & 1) == 0) {
            anim_sprite_disp(a + 20, a + 56, *(u16 *)(a + 16));
        }
        if (b[64] & 1) {
            return;
        }
        anim_sprite_disp(b + 20, b + 56, *(u16 *)(a + 16));
        return;
    }

    p = a + 20;
    sel = tt_player_wk[87];
    if (sel == 0) {
        anim_sprite_disp(p, b + 56, *(u16 *)(b + 16));
        b += 96;
        anim_sprite_disp(p, b + 56, *(u16 *)(b + 16));
        anim_sprite_disp(p, a + 56, *(u16 *)(a + 16));
    } else if (sel < 3) {
        anim_sprite_disp(p, b + 152, *(u16 *)(b + 16));
        anim_sprite_disp(p, a + 56, *(u16 *)(a + 16));
        anim_sprite_disp(p, b + 56, *(u16 *)(b + 16));
    } else {
        anim_sprite_disp(p, b + 56, *(u16 *)(b + 16));
        anim_sprite_disp(p, a + 56, *(u16 *)(a + 16));
        anim_sprite_disp(p, b + 152, *(u16 *)(b + 16));
    }
}
