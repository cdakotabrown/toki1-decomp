/* @PostProc: scheduler_normalizer --block-aware */
/* @SymbolSize: 184 */
#include "types.h"

extern s32 anim_sprite_disp(u16 *a0, u8 *a1, s32 a2);

extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;
extern u8 *tt_cel_buf;

void tt_anim_disp(void)
{
    u8 *flag;
    u8 *s0;
    u8 *s1;
    u8 *s2;
    s32 s3;
    s32 s4;

    flag = tt_player_wk;
    if (flag[83] == 0) {
        return;
    }

    s3 = 0;
    s0 = tt_cel_buf;
    s1 = tt_trail_buf;
    s4 = 3;
    s0 = s0 + 192;

    do {
        s2 = s1 + 20;
        if (s1[0] != 0) {
            anim_sprite_disp((u16 *)s2, s1 + 56, *(u16 *)(s1 + 16));
            anim_sprite_disp((u16 *)s2, s0 + 56, *(u16 *)(s0 + 16));
            s0 = s0 + 96;
            anim_sprite_disp((u16 *)s2, s0 + 56, *(u16 *)(s0 + 16));
        }
        s3 += 1;
        s1 = s1 + 164;
        s0 = s0 + 96;
    } while (s3 != s4);
}
