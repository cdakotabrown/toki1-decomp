/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* tt_scroll_disp - TT overlay, Address: 0x8013D508, Size: 564 bytes (141 insns) */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

typedef struct {
    u8 a, b, c;
} Ent;

extern u8 *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_stage_wk;
extern u8 *tt_db_buf;

extern s32 SetDrawMode(void *, s32, s32, s32, s32);

void tt_scroll_disp(void)
{
    s32 yy;
    u8 *base;
    u8 *res;
    Ent *e;
    PTag *ot;
    u8 *p;
    u8 *q;
    s32 i;
    s32 y;
    s32 n;
    s32 ang;
    s32 hi;
    s32 lo;

    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    ang = (s16)(768 - *(u16 *)(tt_stage_wk + 2));
    res = tt_step_wk;
    hi = ang & -16;
    ot = (PTag *)(base + 0x8000);
    lo = ang & 0xF;
    n = 3;
    for (y = 0; y != 256; y += 16) {
        i = 0;
        yy = y - lo;
        while (i != 16) {
            p = *(u8 **)(base + 4);
            *(u8 **)(base + 4) = p + 16;
            p[7] = 125;
            p[3] = (u8)n;
            *(s16 *)(p + 8) = (s16)((i << 4) - 128);
            *(s16 *)(p + 10) = yy;
            e = (Ent *)(*(u8 **)(*(u8 **)(res + 24) + 4) + *(*(u8 **)(*(u8 **)(res + 24) + 0) + ((y + i + hi) & 0x3FF)) * n);
            p[12] = (u8)((e->b & 7) << 4);
            p[13] = (u8)((((u32)e->b >> 3) << 4) + (e->a << 7));
            *(s16 *)(p + 14) = (s16)((e->c & 0xF) + ((((u32)e->c >> 4) + 240) << 6));
            ((PTag *)p)->addr = ot[15].addr;
            ot[15].addr = (u32)p;
            i++;
        }
    }

    q = *(u8 **)(base + 4);
    *(u8 **)(base + 4) += 12;
    SetDrawMode(q, 0, 0, 14, 0);
    ((PTag *)q)->addr = ot[15].addr;
    ot[15].addr = (u32)q;
}
