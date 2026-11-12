/* tt_crowd_anim - TT overlay, Address: 0x8013F868, Size: 272 bytes (68 insns) */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 pad0[2];
    u8 frame;
    u8 pad3[17];
} Spr;

typedef struct {
    u8 pad0[80];
    Spr s[10];
    u8 pad[588];
    u16 tick;
} Crowd;

extern Crowd *tt_boss_wk;

tt_crowd_anim()
{
    Crowd *p;
    u8 c1, c2, c3;

    p = tt_boss_wk;
    p->s[0].frame = ((p->tick >> 2) & 1) + 14;
    p->s[1].frame = ((p->tick >> 2) & 1) + 14;
    if ((p->tick & 7) == 7) {
        if ((c1 = ++p->s[2].frame) >= 52) {
            c1 = p->s[2].frame = 46;
        }
        p->s[3].frame = c1;
        if ((c2 = ++p->s[4].frame) >= 46) {
            c2 = p->s[4].frame = 40;
        }
        p->s[5].frame = c2;
        if ((c3 = ++p->s[6].frame) >= 40) {
            c3 = p->s[6].frame = 34;
        }
        p->s[7].frame = c3;
    }
    if ((p->tick & 15) == 15) {
        if (p->s[8].frame & 1) {
            p->s[8].frame--;
        } else {
            p->s[8].frame++;
        }
    }
    p->s[9].frame = (p->tick >> 4) & 1;
    p->tick++;
}
