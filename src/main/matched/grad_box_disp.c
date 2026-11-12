/* Address: 0x80085890 */
/* Size: 484 bytes (121 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=15:a1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x4C --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x158 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x148 */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 r3, g3, b3, pad3;
    s16 x3, y3;
} POLY_G4;

extern s32 activeBuff;
extern s16 screen_w[];
extern u32 ot_buff0[][256];

extern POLY_G4 *prim_alloc();
extern void SetPolyG4();
extern void SetSemiTrans();
extern void AddPrim();

void grad_box_disp(x, y, w, h, ot, c0, c1)
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 ot;
    u32 c0;
    u32 c1;
{
    s32 unused;
    POLY_G4 *p;

    p = prim_alloc(36, activeBuff);
    SetPolyG4(p);
    SetSemiTrans(p, 0);
    p->r0 = (c1 >> 16) & 0xff; p->g0 = (c1 >> 8) & 0xff; p->b0 = c1 & 0xff;
    p->r1 = (c1 >> 16) & 0xff; p->g1 = (c1 >> 8) & 0xff; p->b1 = c1 & 0xff;
    p->r2 = (c0 >> 16) & 0xff; p->g2 = (c0 >> 8) & 0xff; p->b2 = c0 & 0xff;
    p->r3 = (c0 >> 16) & 0xff; p->g3 = (c0 >> 8) & 0xff; p->b3 = c0 & 0xff;
    p->x0 = screen_w[activeBuff + 10] + x;
    p->y0 = screen_w[activeBuff + 12] + y;
    p->x1 = screen_w[activeBuff + 10] + x + w;
    p->y1 = screen_w[activeBuff + 12] + y;
    p->x2 = screen_w[activeBuff + 10] + x;
    p->y2 = screen_w[activeBuff + 12] + y + h;
    p->x3 = screen_w[activeBuff + 10] + x + w;
    p->y3 = screen_w[activeBuff + 12] + y + h;
    AddPrim(&ot_buff0[activeBuff][ot], p);
}
