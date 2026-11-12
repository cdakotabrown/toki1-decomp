/* @CompileEnv: IDO52_FORCE_WEB_COLOR=63:v1,58:a3,60:t0,62:t1 */
/* @CompileFlags: -cckr */
/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x160 */
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
extern s32 D_800E9600;
extern s16 D_800B0860[];
extern u32 D_800B2580[][16];
extern POLY_G4 *func_80019D00();

void func_80144B24(x, y, w, h, c, pri, abe)
s32 x, y, w, h;
u32 c;
s32 pri, abe;
{
    s32 pad;
    POLY_G4 *p;

    p = func_80019D00(36, D_800E9600);
    func_80067678(p);
    func_800675A8(p, abe);
    p->r0 = (c >> 16) & 0xff, p->g0 = (c >> 8) & 0xff, p->b0 = c;
    p->r1 = (c >> 16) & 0xff, p->g1 = (c >> 8) & 0xff, p->b1 = c;
    p->r2 = (c >> 16) & 0xff, p->g2 = (c >> 8) & 0xff, p->b2 = c;
    p->r3 = (c >> 16) & 0xff, p->g3 = (c >> 8) & 0xff, p->b3 = c;
    p->x0 = D_800B0860[D_800E9600 + 10] + x;
    p->y0 = D_800B0860[D_800E9600 + 12] + y;
    p->x1 = D_800B0860[D_800E9600 + 10] + x + w;
    p->y1 = D_800B0860[D_800E9600 + 12] + y;
    p->x2 = D_800B0860[D_800E9600 + 10] + x;
    p->y2 = D_800B0860[D_800E9600 + 12] + y + h;
    p->x3 = D_800B0860[D_800E9600 + 10] + x + w;
    p->y3 = D_800B0860[D_800E9600 + 12] + y + h;
    func_800674F4(&D_800B2580[D_800E9600][pri], p);
}
