/* Address: 0x80135A54 */
/* Size: 112 bytes (28 insns) */
#include "types.h"

typedef union {
    s32 w;
    struct {
        u16 lo;
        s16 hi;
    } h;
} Fix16;

typedef struct {
    u8    pad0[4];
    s16   h4;
    s16   h6;
    s16   h8;
    s16   h10;
    u8    pad0C;
    u8    b13;
    u8    b14;
    u8    b15;
    u8    pad10[4];
    s16   h20;
    s16   h22;
    u8    pad18[8];
    Fix16 f32;
    Fix16 f36;
} Rec;

extern Rec *tt_boss_wk;

void tt_slide_in(void) {
    Rec *s;

    s = tt_boss_wk;
    s->f32.w = 0;
    s->f36.w += 0x10000;
    s->h20 = s->f32.h.hi;
    s->h22 = s->f36.h.hi;
    if (s->h22 >= 80) {
        s->f36.w = 0x500000;
        s->h22 = s->f36.h.hi;
        s->h4 = 48;
        s->h10 = 0;
        s->h8 = 0;
        s->h6 = 0;
        s->b15 = 0;
        s->b14 = 0;
        s->b13 = 0;
    }
}
