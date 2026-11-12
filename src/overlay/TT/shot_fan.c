/* @SymbolSize: 448 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  b0;
    u8  pad01[3];
    u16 h4;
    u16 h6;
    u16 h8;
    u16 h10;
    u8  pad0C;
    u8  b13;
    u8  b14;
    u8  b15;
    u8  pad10[16];
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u8  pad30[56];
} Shot;

extern u8   *tt_player_wk;
extern Shot *tt_shot_buf;

extern Shot *get_tt_shot_free();
extern void tt_shot_sprite_set(Shot *p, u16 mode, s32 k);

void shot_fan(x, y, step, scale, mode)
s16 x;
s16 y;
s32 step;
s32 scale;
u16 mode;
{
    s32 unused;
    u8 *ball;
    Shot *p;
    s32 i;
    s32 ang;

    ball = tt_player_wk;
    p = tt_shot_buf;
    for (i = 0; i < 9; i++) {
        p = get_tt_shot_free(p, p + 128);
        if (p == 0) {
            return;
        }
        p->x = x << 16;
        p->y = y << 16;
        ang = get_tt_aim_angle(p, ball);
        if (i != 0) {
            if (i & 1) {
                ang -= (i / 2 + 1) * step;
            } else {
                ang += (i / 2) * step;
            }
        }
        p->vx = rcos(ang) * scale;
        p->vy = rsin(ang) * scale;
        switch (mode) {
        case 160:
            tt_shot_sprite_set(p, 160, 18);
            break;
        case 170:
            tt_shot_sprite_set(p, 170, 23);
            break;
        }
        p->h4 = 64;
        p->h6 = p->h8 = p->h10 = 0;
        p->b13 = p->b14 = p->b15 = 0;
    }
}
