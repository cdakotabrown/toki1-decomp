/* @CompileFlags: -cckr */
/* @SymbolSize: 204 */
#include "types.h"

typedef struct {
    u8 flag;
    u8 pad1[19];
    s16 x;
    s16 y;
    u8 pad2[10];
    s16 bx;
    s16 pad3;
    s16 by;
    u8 pad4[16];
    u8 rec[24][20];
    u8 pad5[48];
    s16 dx[24];
    s16 dy[24];
    u16 z[24];
    u8 pad6[56];
    u8 on[24];
} T;

extern T *tt_boss_wk;
extern void anim_sprite_disp();

void tt_unit_disp()
{
    T *p;
    int i;

    p = tt_boss_wk;
    if (p->flag == 0) {
        return;
    }
    for (i = 0; i < 24; i++) {
        if (p->on[i] != 0) {
            p->x = p->dx[i] + p->bx;
            p->y = p->dy[i] + p->by;
            anim_sprite_disp(&p->x, p->rec[i], p->z[i]);
        }
    }
    p->x = p->bx;
    p->y = p->by;
}
