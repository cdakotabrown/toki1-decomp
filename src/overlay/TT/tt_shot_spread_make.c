/* @CompileEnv: IDO52_AS1_TIE_TAKE=223 */
/* tt_shot_spread_make - TT overlay, Address: 0x80133FB0, Size: 456 bytes (114 insns) */
#include "types.h"

extern u8 *tt_shot_buf;
extern u8 *tt_player_wk;

extern u8 *get_tt_shot_free(u8 *a0, u8 *a1);
extern s32 get_tt_aim_angle(u8 *a0, u8 *a1);
extern void tt_shot_sprite_set(u8 *a0, s32 a1, s32 a2);
extern s32 rcos(s32 angle);
extern s32 rsin(s32 angle);

void tt_shot_spread_make(s16 posX, s16 posY, s32 spread, s32 mul, u16 kind)
{
    u8 *p;
    u8 *ctx;
    s32 i;
    s32 ang;
    s32 base;
    u32 k;

    p = tt_shot_buf;
    ctx = tt_player_wk;
    k = kind;
    i = 0;
    do {
        p = get_tt_shot_free(p, p + 13312);
        if (p == 0) {
            return;
        }
        *(s32 *)(p + 32) = posX << 16;
        *(s32 *)(p + 36) = posY << 16;
        base = get_tt_aim_angle(p, ctx);
        ang = base;
        switch (i) {
        case 1:
            ang = base - spread;
            break;
        case 2:
            ang = base + spread;
            break;
        }
        *(s32 *)(p + 40) = rcos(ang) * mul;
        *(s32 *)(p + 44) = rsin(ang) * mul;
        switch (k) {
        case 160:
            tt_shot_sprite_set(p, 160, 18);
            break;
        case 162:
            tt_shot_sprite_set(p, 162, 20);
            break;
        case 168:
            tt_shot_sprite_set(p, 168, 21);
            break;
        case 170:
            tt_shot_sprite_set(p, 170, 23);
            break;
        }
        i++;
        *(s16 *)(p + 4) = 64;
        *(s16 *)(p + 10) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[15] = 0;
        p[14] = 0;
        p[13] = 0;
    } while (i != 3);
}
