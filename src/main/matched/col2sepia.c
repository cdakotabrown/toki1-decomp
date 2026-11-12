/* Address: 0x8004C50C */
/* Size: 572 bytes (143 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2, K&R mode (-cckr) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 r0:5;
    u32 g0:5;
    u32 b0:5;
    u32 m0:1;
    u32 r1:5;
    u32 g1:5;
    u32 b1:5;
    u32 m1:1;
} Clut2;

extern void clut_gray_set(s32 arg0);

void col2sepia(s32 arg0, Clut2 *clut, s32 dr, s32 dg, s32 db)
{
    s32 i;
    u32 v;
    Clut2 *p;

    clut_gray_set(arg0);
    p = clut;

    for (i = 0; i < 128; i++) {
        v = p->r0 + p->g0 + p->b0;
        if (v != 0) {
            if (p->r0 + dr >= 32) {
                p->r0 = 31;
            } else {
                p->r0 += dr;
            }
            if (p->g0 + dg >= 32) {
                p->g0 = 31;
            } else {
                p->g0 += dg;
            }
            if (p->b0 + db >= 32) {
                p->b0 = 31;
            } else {
                p->b0 += db;
            }
        }
        v = p->r1 + p->g1 + p->b1;
        if (v != 0) {
            if (p->r1 + dr >= 32) {
                p->r1 = 31;
            } else {
                p->r1 += dr;
            }
            if (p->g1 + dg >= 32) {
                p->g1 = 31;
            } else {
                p->g1 += dg;
            }
            if (p->b1 + db >= 32) {
                p->b1 = 31;
            } else {
                p->b1 += db;
            }
        }
        p++;
    }
}
