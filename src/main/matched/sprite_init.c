/* Address: 0x80041774 */
/* Size: 152 bytes (38 instructions) */
/* Region: Game */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

#include "tokimeki/sprite.h"

extern SPRITE_REC Sprites[];

extern u8 screen_w[];

void sprite_init(s32 a0, s32 a1) {
    s32 i;

    for (i = a0; i < a1; i++) {
        SPRITE_REC *e = &Sprites[i];
        e->attribute  = 0x80000000u;
        e->x  = 0;
        e->y  = 0;
        e->w  = 0;
        e->h  = 0;
        e->tpage  = 0;
        e->u  = 0;
        e->v  = 0;
        e->cx = 0;
        e->cy = 0;
        e->r = 0;
        e->g = 0;
        e->b = 0;
        e->mx = 0;
        e->my = 0;
        e->scalex = 0;
        e->scaley = 0;
        e->rotate = 0;
        screen_w[0x10A5 + i] = 0;
    }
}
