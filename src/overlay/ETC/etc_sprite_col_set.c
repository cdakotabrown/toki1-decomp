/* @CompileFlags: -cckr */
/* Address: 0x80144144 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

typedef struct {
    u8 pad0[20];
    u8 b20;
    u8 b21;
    u8 b22;
    u8 pad23[13];
} Spr;

extern Spr Sprites[];

etc_sprite_col_set(idx, val)
    s32 idx;
    s32 val;
{
    u8 v;
    Spr *p;

    if (val > 128) {
        v = 128;
    } else if (val < 0) {
        v = 0;
    } else {
        v = val;
    }
    p = &Sprites[idx];
    p->b22 = v;
    p->b21 = v;
    p->b20 = v;
}
