/* tt_ball_spin - TT overlay, Address: 0x8014A188, Size: 284 bytes (71 insns) */
/* @CompileFlags: -cckr */
#include "types.h"

typedef union {
    s32 w;
    struct {
        s16 lo;
        s16 hi;
    } h;
} Fix;

typedef struct {
    u8 pad0[20];
    s16 x;
    s16 y;
    u8 pad18[8];
    Fix fx;
    Fix fy;
    u8 pad28[30];
    u8 sh0;
    u8 sh1;
    u8 pad48[10];
    u16 phase;
    u8 pad54[6];
    u16 period;
} Ball;

typedef struct {
    u8 pad0[3200];
    Ball ball;
} World;

typedef struct {
    s16 f0;
    u16 f2;
    s16 f4;
    s16 f6;
} Pos;

extern World *tt_anim_buf;
extern Pos *tt_step_wk;
extern Pos *tt_scale_buf;

tt_ball_spin()
{
    Ball *p;
    Pos *c;
    Pos *d;
    s16 q;

    c = tt_step_wk;
    p = &tt_anim_buf->ball;
    d = tt_scale_buf;
    if (p->period >= 4096) {
        c->f2++;
    }
    p->fx.w = 0x18000000 / p->period;
    p->fy.w = 0x6C0000 - 13824 / p->period;
    p->x = p->fx.h.hi;
    p->y = p->fy.h.hi;
    d->f4 = p->x;
    d->f6 = p->y + 24;
    q = 0x180000 / p->period;
    d->f2 = q;
    d->f0 = q;
    if (p->phase & 1) {
        p->sh0 = 128;
        p->sh1 = 0;
    } else {
        p->sh0 = 0;
        p->sh1 = 128;
    }
    p->phase++;
    p->period += 64;
}
