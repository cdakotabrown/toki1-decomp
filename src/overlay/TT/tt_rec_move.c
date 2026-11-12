/* @SymbolSize: 612 */
/* @CompileFlags: -cckr */
#include "types.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

typedef union {
    s32 l;
    struct {
        u16 lo;
        s16 hi;
    } w;
} Fix;

typedef struct {
    u8  b0;
    u8  pad01[19];
    s16 sx;
    s16 sy;
    u8  pad18[8];
    Fix x;
    Fix y;
    s32 dx;
    s32 dy;
} Ball;

typedef struct {
    u8  b0;
    u8  pad01[19];
    s16 sx;
    s16 sy;
    u8  pad18[44];
    u16 h68;
    u8  pad46[18];
} Obj;

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
    u8  pad10[4];
    s16 sx;
    s16 sy;
    u8  pad18[96];
} Actor;

extern Obj   *tt_recs_buf;
extern Actor *tt_obj_pool;

void tt_rec_move(p, idx)
Ball *p;
s32 idx;
{
    Obj *t;
    Actor *q;
    s32 d;
    s32 i;

    t = &tt_recs_buf[idx];
    p->x.l += p->dx;
    p->y.l += p->dy;
    p->sx = p->x.w.hi;
    p->sy = p->y.w.hi;
    d = ABS(p->sx - t->sx);
    if (d < 2) {
        p->dx = 0;
    } else {
        p->dx = (t->sx - p->sx) << 12;
        if (p->sx < t->sx) {
            d = 0x20000;
        } else {
            d = -0x20000;
        }
        p->dx += d;
    }
    if (p->sy < t->sy || p->sy < 0) {
        p->b0 = 0;
        t->b0 = 0;
        if (t->h68 != 0xFFFF) {
            q = &tt_obj_pool[t->h68];
            if (q->h4 == 64) {
                q->h4 = 112;
                q->h6 = q->h8 = q->h10 = 0;
                q->b13 = q->b14 = q->b15 = 0;
            }
        } else {
            for (i = 0, q = tt_obj_pool; i < 32; i++, q++) {
                if (q->h4 == 64 && ABS(t->sx - q->sx) < 8 &&
                    ABS(t->sy - q->sy) < 8) {
                    q->h4 = 112;
                    q->h6 = q->h8 = q->h10 = 0;
                    q->b13 = q->b14 = q->b15 = 0;
                }
            }
        }
    }
}
