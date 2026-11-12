/* Address: 0x8004A360 */
/* Size: 348 bytes (87 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
} LINE_G2;

extern s32 activeBuff;
extern s16 screen_w[];
extern u32 ot_buff0[][256];

extern LINE_G2 *prim_alloc();

void line_set(x0, y0, x1, y1, c0, c1, ot)
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    u32 c0;
    u32 c1;
    u8 ot;
{
    LINE_G2 *p;

    p = prim_alloc(20, activeBuff);
    SetLineG2(p);
    if (ot & 0x80) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    p->r0 = (c0 >> 16) & 0xff; p->g0 = (c0 >> 8) & 0xff; p->b0 = c0 & 0xff;
    p->r1 = (c1 >> 16) & 0xff; p->g1 = (c1 >> 8) & 0xff; p->b1 = c1 & 0xff;
    p->x0 = screen_w[activeBuff + 10] + x0;
    p->y0 = screen_w[activeBuff + 12] + y0;
    p->x1 = screen_w[activeBuff + 10] + x1;
    p->y1 = screen_w[activeBuff + 12] + y1;
    AddPrim(&ot_buff0[activeBuff][ot & 0x7f], p);
}
