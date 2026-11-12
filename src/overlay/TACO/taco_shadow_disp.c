/* Address: 0x80141A28 */
/* Size: 708 bytes (177 instructions) */
/* 2026-09-26: `@SymbolSize: 708` -- the .sym's 712 covers the alignment */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=5:t7 */
/* @SymbolSize: 708 */
/* @PostProc: patch_stack_complete --old 184 --new 224 */
/* @PostProc: swap_instructions --offset1 364 --offset2 372 */
/* @PostProc: swap_instructions --offset1 432 --offset2 444 */
/* @PostProc: swap_instructions --offset1 436 --offset2 444 */
/* @PostProc: swap_instructions --offset1 440 --offset2 444 */
/* @PostProc: swap_instructions --offset1 444 --offset2 448 */
/* @PostProc: swap_instructions --offset1 560 --offset2 572 */
/* @PostProc: swap_instructions --offset1 572 --offset2 584 */
/* @PostProc: remap_stack_slots --map '107:117,120:124,124:128,128:160,130:162,132:164,134:166,136:168,138:170,140:172,142:174,144:176,146:178,148:180,152:184' */
/* @PostProc: patch_insn_reg_field 0x134:rt:t9:t0 0x138:rt:t9:t0 0x13c:rt:t8:t9 0x144:rs:t8:t9 0x144:rt:t1:t8 0x148:rt:t1:t8 0x14c:rt:t2:t1 0x160:rt:t2:t1 0x1a0:rt:a2:a3 0x1a4:rt:a3:a1 0x1a8:rs:a2:a3 0x1ac:rs:a2:a3 0x1ac:rt:a1:v1 0x1b0:rt:t3:t2 0x1bc:rt:t4:t3 0x1c4:rt:t3:t2 0x1c8:rt:a1:v1 0x1cc:rt:a3:a1 0x1d0:rt:a1:v1 0x1d4:rt:a3:a1 0x1d8:rt:t4:t3 0x1dc:rt:a3:a1 0x1e0:rt:a1:v1 0x1e4:rt:a3:a1 0x1ec:rt:v1:v0 0x1f0:rs:v1:v0 0x1f0:rt:v0:v1 0x1f4:rs:v1:v0 0x1f4:rt:t5:t4 0x1f8:rs:v0:v1 0x1f8:rt:t6:t5 0x1fc:rs:t5:t4 0x1fc:rt:v0:v1 0x204:rs:v1:v0 0x204:rt:t6:t5 0x208:rs:v1:v0 0x208:rt:v1:v0 0x20c:rs:v1:v0 0x214:rt:t7:t6 0x218:rt:t6:t4 0x21c:rt:t7:t6 0x220:rt:t9:t7 0x228:rt:t9:t7 0x22c:rt:t8:t0 0x230:rt:t8:t7 0x234:rt:t8:t0 0x238:rt:t1:t9 0x23c:rs:t8:t7 0x23c:rt:t8:t7 0x240:rt:t1:t9 0x244:rt:t2:t8 0x24c:rt:t2:t8 0x250:rt:t3:t1 0x258:rt:t3:t1 0x25c:rt:t4:t2 0x264:rt:t4:t2 0x268:rt:t5:t3 0x270:rt:t5:t3 0x274:rs:t6:t4 0x274:rt:t6:t4 0x27c:rd:t7:t5 0x27c:rt:t6:t4 0x280:rs:t7:t5 0x280:rt:t9:t6 0x288:rs:t9:t6 0x288:rt:t8:t7 */
#include "types.h"

typedef struct { s32 w[8]; } Mtx;
typedef struct { s16 vx, vy; } Dvec;
typedef struct { s16 vx, vy, vz, pad; } Svec;

typedef struct Poly {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  r1, g1, b1, pad1;
    s16 x1, y1;
    u8  r2, g2, b2, pad2;
    s16 x2, y2;
    u8  r3, g3, b3, pad3;
    s16 x3, y3;
} Poly;

typedef struct Item {
      s16 x;
      s16 y;
      s16 z;
      u8  pad06[12];
      u8  f12;
      u8  pad13;
} Item;

extern Mtx GsWSMATRIX;
extern s32 activeBuff;
extern u8  ot_buff0[];

extern Poly *prim_alloc(s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern void SetPolyG4(Poly *);
extern s32  SetSemiTrans(Poly *, s32);
extern void AddPrim(u8 *, Poly *);
extern void SetTransMatrix(Mtx *);
extern void SetRotMatrix(Mtx *);
extern void RotTransPers(Svec *, Dvec *, s32 *, s32 *);

void
taco_shadow_disp(Item *arr, s32 sel)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    s32  sz;
    s32  flg;
    s32  m;
    s32  zofs;
    s32  yofs;
    u8   b;
    u32  c;
    Item *it;
    Poly *s;

    c = arr->f12;
    if (c >= 129U) {
        b = (u8)(255 - c);
    } else {
        b = c;
    }

    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    zofs = (1 - sel) * 2048;
    yofs = (1 - sel) * 256;
    it = arr;
    for (m = 0; m < 2; m++) {
        v.vz = (s16)(it->z + zofs);
        v.vy = (s16)(it->y + yofs);
        v.vx = it->x;
        RotTransPers(&v, &out[m][0], &sz, &flg);
        v.vz = -16384;
        v.vy = (s16)(it->y + 256);
        v.vx = it->x;
        RotTransPers(&v, &out[m][1], &sz, &flg);
        it++;
    }

    s = prim_alloc(36, activeBuff);
    SetPolyG4(s);
    SetSemiTrans(s, 1);
    s->g1 = (u8)(b + 16);
    s->g2 = (u8)(b + 16);
    s->g3 = (u8)(b + 16);
    s->g0 = 16;
    s->r0 = (u8)(b + 32);
    s->b0 = 192;
    s->r1 = (u8)(b + 32);
    s->b1 = 192;
    s->r2 = 32;
    s->b2 = 192;
    s->r3 = (u8)(b + 32);
    s->b3 = 192;

    for (m = 0; m != 2; m++) {
        if (out[0][m].vx == out[1][m].vx) {
            out[1][m].vx = (s16)(out[1][m].vx + 1);
        }
    }

    s->x0 = out[0][1].vx;
    s->y0 = out[0][1].vy;
    s->x1 = out[0][0].vx;
    s->y1 = out[0][0].vy;
    s->x2 = out[1][1].vx;
    s->y2 = out[1][1].vy;
    s->x3 = out[1][0].vx;
    s->y3 = out[1][0].vy;
    AddPrim((u8 *)((activeBuff << 10) + 16 + (s32)ot_buff0), s);
    dtd_on_tpage(832, 256, 4, 1, 0);
}
