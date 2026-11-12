/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=9:a1,66:a3,68:t0,70:t1 */
/* Address: 0x8013FCE8, Size: 476 bytes (119 insns) */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    short x0, y0;
    u8 r1, g1, b1, p1;
    short x1, y1;
    u8 r2, g2, b2, p2;
    short x2, y2;
    u8 r3, g3, b3, p3;
    short x3, y3;
} LINE_G4;

extern LINE_G4 *prim_alloc();
extern int activeBuff;
extern short screen_w[];
extern u32 ot_buff0[][256];

geko_rect_disp(x, y, w, h, col, idx, abe)
int x, y, w, h;
u32 col;
int idx, abe;
{
    LINE_G4 *p;
    LINE_G4 *pp[1];
    int t;

    pp[0] = prim_alloc(36, activeBuff);
    SetPolyG4(pp[0]);
    SetSemiTrans(pp[0], abe);
    p = pp[0];
    p->r0 = (col >> 16) & 0xff, p->g0 = (col >> 8) & 0xff, p->b0 = col;
    p->r1 = (col >> 16) & 0xff, p->g1 = (col >> 8) & 0xff, p->b1 = col;
    p->r2 = (col >> 16) & 0xff, p->g2 = (col >> 8) & 0xff, p->b2 = col;
    p->r3 = (col >> 16) & 0xff, p->g3 = (col >> 8) & 0xff, p->b3 = col;
    t = x + y + w;
    p->x0 = screen_w[activeBuff + 10] + x;
    p->y0 = screen_w[activeBuff + 12] + y;
    p->x1 = screen_w[activeBuff + 10] + x + w;
    p->y1 = screen_w[activeBuff + 12] + y;
    p->x2 = screen_w[activeBuff + 10] + x;
    p->y2 = screen_w[activeBuff + 12] + y + h;
    p->x3 = screen_w[activeBuff + 10] + x + w;
    p->y3 = screen_w[activeBuff + 12] + y + h;
    AddPrim(&ot_buff0[activeBuff][idx]);
}
