/* Address: 0x80132000, Size: 240 bytes (60 insns) */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=27:v0,23:v1 */
#include "types.h"

typedef struct {
    u8 dur, pos, pat;
} TtFrame;

typedef struct {
    u8 *script;
    u8 pattern;
    u8 pad5;
    u8 frame;
    u8 pad7;
    u8 flags;
    u8 timer;
} TtAnim;

void tt_anim(TtAnim *a, u8 *x, u8 *y)
{
    u32 f;
    u8 *s;
    u8 *p;
    u8 c;

    f = a->flags;
    if (f & 0x80) {
        a->flags = f & ~0x80;
        a->frame = 0;
        goto show;
    }
    if (f & 0x20) {
        a->flags = f & ~0x20;
        goto show;
    }
    if (f & 0x40) {
        return;
    }
    if (--a->timer != 0) {
        return;
    }
    a->frame++;
show:
    s = a->script;
    p = (u8 *)((TtFrame *)s + a->frame);
    c = *p;
    a->timer = c;
    if (c == 254) {
        a->frame = 0;
        a->timer = *s;
        p = s;
        a->flags |= 1;
    }
    *x = ((p[1] & 7) << 4) + 128;
    *y = (p[1] & ~7) << 1;
    p += 2;
    if (*p != 255) {
        a->pattern = *p;
    }
}
