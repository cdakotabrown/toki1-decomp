/* anim_cel_step - TT overlay, Address: 0x801478C0, Size: 328 bytes (82 insns). */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 pad0[2];
    u8 dur;
    u8 ctrl;
} Cel;

typedef struct {
    Cel *cels;
} Anim;

typedef struct {
    u8 pad0[5];
    u8 anim;
    u8 frame;
    u8 pad7;
    u8 flags;
    u8 timer;
} Obj;

extern Anim *tt_anim_tbl[];

anim_cel_step(o)
Obj *o;
{
    Cel *cel;
    int fr;
    int c;

    if (o->flags & 0x80) {
        o->flags &= ~0x80;
        o->frame = 0;
        cel = tt_anim_tbl[o->anim]->cels;
        o->timer = cel->dur;
        return;
    }
    if (o->flags & 0x20) {
        o->flags &= ~0x20;
        cel = &tt_anim_tbl[o->anim]->cels[o->frame];
        o->timer = cel->dur;
        return;
    }
    if (o->flags & 0x40) {
        return;
    }
    if (--o->timer == 0) {
        fr = o->frame;
        cel = &tt_anim_tbl[o->anim]->cels[fr];
        c = cel->ctrl;
        if (c == 0) {
            o->frame = fr + 1;
            cel = &tt_anim_tbl[o->anim]->cels[o->frame];
            o->timer = cel->dur;
            return;
        }
        if (c == 255) {
            o->flags |= 1;
            return;
        }
        o->frame = fr - c;
        cel = &tt_anim_tbl[o->anim]->cels[o->frame];
        o->timer = cel->dur;
    }
}
