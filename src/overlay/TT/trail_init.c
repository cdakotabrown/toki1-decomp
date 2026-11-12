/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny,lod:3:deny,lod:4:deny IDO52_AS1_NO_PASS1_PROP=10,11,12,13 */
/* trail_init - TT overlay, Address: 0x80147FC0, Size: 236 bytes (59 insns) */
#include "types.h"

typedef struct {
    u8  f0;
    u8  f1[3];
    s16 f4;
    s16 f6;
    s16 f8;
    s16 fA;
    u8  fC;
    u8  fD;
    u8  fE;
    u8  fF;
    u8  f10[4];
    s16 x;
    s16 y;
    u8  f18[56];
    s16 hx[16];
    s16 hy[16];
} Trail;

extern Trail *tt_player_wk;

void trail_init(p)
Trail *p;
{
    Trail *src;
    int i;
    int x;
    int y;

    src = tt_player_wk;
    y = 0; x = 0;
    p->f0 = 1;
    p->f4 = 64;
    p->fE = x; p->fD = x; p->f8 = y; p->f6 = y;
    p->fA = 0; p->fF = 0;
    p->x = src->x;
    p->y = src->y;
    for (i = 0; i < 10; i++) {
        p->hx[i] = src->x;
        p->hy[i] = src->y;
    }
}
