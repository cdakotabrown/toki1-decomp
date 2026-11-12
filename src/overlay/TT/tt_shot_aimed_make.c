/* tt_shot_aimed_make - TT overlay, Address: 0x80133D14, Size: 284 bytes (71 insns) */
#include "types.h"

extern u8 *tt_shot_buf;
extern s32 tt_player_wk;

extern s32 get_tt_shot_free(u8 *a0, u8 *a1);
extern s32 get_tt_aim_angle(s32 a0, s32 a1);
extern s32 rcos(s32 a0);
extern s32 rsin(s32 a0);
extern void tt_shot_sprite_set(s32 a0, s32 a1, s32 a2);

void tt_shot_aimed_make(s32 arg0, s32 arg1, s32 arg2, volatile u16 arg3)
{
    u8 *base;
    s32 g;
    s32 r;
    s32 t;
    u32 sel;
    s32 s0;

    base = tt_shot_buf;
    g = tt_player_wk;
    s0 = get_tt_shot_free(base, base + 13312);
    if (s0 == 0) {
        return;
    }
    *(s32 *)(s0 + 32) = (s32)(s16)arg0 << 16;
    *(s32 *)(s0 + 36) = (s32)(s16)arg1 << 16;
    r = get_tt_aim_angle(s0, g);
    *(s32 *)(s0 + 40) = rcos(r) * arg2;
    t = rsin(r);
    sel = arg3;
    *(s32 *)(s0 + 44) = t * arg2;

    switch (sel) {
    case 160:
        tt_shot_sprite_set(s0, 160, 18);
        break;
    case 168:
        tt_shot_sprite_set(s0, 168, 21);
        break;
    }

    *(s16 *)(s0 + 4) = 64;
    *(s16 *)(s0 + 10) = 0;
    *(s16 *)(s0 + 8) = 0;
    *(s16 *)(s0 + 6) = 0;
    *(u8 *)(s0 + 15) = 0;
    *(u8 *)(s0 + 14) = 0;
    *(u8 *)(s0 + 13) = 0;
}
