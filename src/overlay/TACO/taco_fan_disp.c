/* @CompileFlags: -cckr */
/* @PostProc: remap_stack_slots --map '88:92' */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Pt;

typedef struct {
    u8  pad0[4];
    u8  r0, g0, b0, pad7;
    s16 x0, y0;
    u8  r1, g1, b1, pad15;
    s16 x1, y1;
    u8  r2, g2, b2, pad23;
    s16 x2, y2;
} Prim;

extern s32 activeBuff;
extern u32 ot_buff0[][256];

extern Prim *prim_alloc();
extern s32 rsin();
extern s32 rcos();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void SetPolyG3();

void taco_fan_disp(out, unused, rgb, amp)
Prim **out;
s32 unused;
u32 rgb;
s32 amp;
{
    Pt pt[42];
    Prim **q;
    s32 i;
    u8 r, g, b;

    for (i = 0; i < 42; i++) {
        if (i == 0) {
            pt[0].x = 256;
            pt[0].y = 120;
            pt[0].z = 0;
        } else {
            pt[i].x = rcos(i * 0x9000 / 360) * amp / 4096 + 256;
            pt[i].y = rsin(i * 0x9000 / 360) * amp / 4096 + 120;
            pt[i].z = 0;
        }
    }
    b = rgb & 0xFF;
    g = (rgb >> 8) & 0xFF;
    r = (rgb >> 16) & 0xFF;
    q = out;
    for (i = 0; i < 40; i++) {
        *q = prim_alloc(28, activeBuff);
        SetPolyG3(*q);
        SetSemiTrans(*q, 1);
        SetShadeTex(*q, 1);
        if (activeBuff == 0) {
            (*q)->x0 = pt[i + 1].x;
            (*q)->y0 = pt[i + 1].y + 240;
            (*q)->x1 = pt[0].x;
            (*q)->y1 = pt[0].y + 240;
            (*q)->x2 = pt[i + 2].x;
            (*q)->y2 = pt[i + 2].y + 240;
        }
        if (activeBuff == 1) {
            (*q)->x0 = pt[i + 1].x;
            (*q)->y0 = pt[i + 1].y;
            (*q)->x1 = pt[0].x;
            (*q)->y1 = pt[0].y;
            (*q)->x2 = pt[i + 2].x;
            (*q)->y2 = pt[i + 2].y;
        }
        (*q)->r0 = (r & 0xFF) - 37;
        (*q)->g0 = (g & 0xFF) - 37;
        (*q)->b0 = (b & 0xFF) - 37;
        (*q)->r1 = r;
        (*q)->g1 = g;
        (*q)->b1 = b;
        (*q)->r2 = (r & 0xFF) - 37;
        (*q)->g2 = (g & 0xFF) - 37;
        (*q)->b2 = (b & 0xFF) - 37;
        AddPrim(&ot_buff0[activeBuff][4], *q);
        q++;
    }
}
