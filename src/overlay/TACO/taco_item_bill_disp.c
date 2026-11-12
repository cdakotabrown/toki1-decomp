/* Address: 0x80142378 */
/* Size: 880 bytes (220 instructions) */
/* @PostProc: patch_stack_complete --old 200 --new 208 */
/* @PostProc: swap_instructions --offset1 272 --offset2 276 */
/* @PostProc: swap_instructions --offset1 276 --offset2 280 */
/* @PostProc: swap_instructions --offset1 280 --offset2 284 */
/* @PostProc: swap_instructions --offset1 448 --offset2 460 */
/* @PostProc: swap_instructions --offset1 756 --offset2 780 */
/* @PostProc: swap_instructions --offset1 768 --offset2 780 */
/* @PostProc: remap_stack_slots --map '72:100,112:96,120:84,124:128,132:120,136:124,142:108' */
/* @PostProc: patch_insn_reg_field 0x80:rt:t0:t1 0x84:rs:t0:t1 0x84:rt:t0:t1 0x88:rs:t0:t1 0x94:rs:t0:t1 0x94:rt:t2:t4 0x9c:rt:t2:t4 0xa0:rs:t0:t1 0xac:rs:t0:t1 0xac:rt:t2:t4 0xb4:rt:t2:t4 0xb8:rs:t0:t1 0xc4:rs:t0:t1 0xc4:rt:t2:t4 0xcc:rt:t2:t4 0xd0:rs:t0:t1 0xdc:rs:t0:t1 0xdc:rt:t2:t4 0xe4:rt:t2:t4 0xf0:rd:t3:t5 0xf4:rd:t3:t5 0xf4:rs:t3:t5 0xf8:rt:t4:t6 0xfc:rs:t4:t6 0xfc:rt:t4:t6 0x100:rd:t3:t5 0x100:rt:t3:t5 0x104:rs:t3:t5 0x104:rt:t4:t6 0x108:rt:t5:t8 0x130:rt:t5:t8 0x14c:rt:t6:t7 0x150:rt:t9:t0 0x154:rd:t8:t9 0x154:rs:t6:t7 0x158:rd:t7:t3 0x158:rs:t8:t9 0x15c:rt:t1:t2 0x160:rt:t7:t3 0x168:rd:s2:s1 0x16c:rd:s1:s2 0x16c:rs:t9:t0 0x16c:rt:t1:t2 0x178:rt:t0:t1 0x17c:rs:s1:s2 0x180:rd:t2:t4 0x180:rs:t0:t1 0x180:rt:s0:s1 0x184:rd:t3:t5 0x184:rs:t2:t4 0x188:rt:t3:t5 0x198:rs:s2:s0 0x198:rt:s2:s0 0x19c:rd:s0:s1 0x19c:rs:s0:s1 0x1a0:rs:s2:s0 0x1a4:rs:s1:s2 0x1a4:rt:s1:s2 0x1a8:rt:t7:t6 0x1ac:rt:t4:t7 0x1b0:rt:t6:t3 0x1b4:rs:t7:t6 0x1b4:rt:t9:t8 0x1b8:rd:t5:t9 0x1b8:rs:t4:t7 0x1bc:rs:t6:t3 0x1bc:rt:t8:t2 0x1c4:rt:t5:t9 0x1c8:rs:t9:t8 0x1d4:rt:t0:t4 0x1e0:rs:t0:t4 0x1e8:rt:v1:a1 0x1ec:rs:v1:a1 0x1f4:rd:t2:t5 0x1f8:rt:t2:t5 0x200:rt:t3:t6 0x204:rt:t4:t7 0x208:rs:t3:t6 0x208:rt:t4:t7 0x214:rt:t5:t9 0x218:rt:t6:t3 0x21c:rs:t6:t3 0x21c:rt:t5:t9 0x228:rt:t8:t2 0x230:rs:t8:t2 0x238:rd:t7:t8 0x240:rs:t7:t8 0x25c:rt:v1:v0 0x260:rt:v1:v0 0x264:rt:v1:v0 0x268:rt:v1:v0 0x290:rt:t1:t4 0x298:rt:t1:t4 0x29c:rt:t0:t5 0x2a4:rt:t0:t5 0x2a8:rt:t2:t6 0x2b0:rs:t2:t6 0x2b0:rt:t3:t7 0x2b4:rt:t3:t7 0x2b8:rt:t4:t9 0x2bc:rt:a2:a0 0x2c0:rt:a2:a0 0x2c4:rt:t4:t9 0x2c8:rt:t5:t3 0x2cc:rt:a2:a0 0x2d0:rs:t5:t3 0x2d0:rt:t6:t2 0x2d4:rt:t6:t2 0x2dc:rt:t5:t2 0x2e4:rt:t7:t1 0x2ec:rt:t7:t1 0x2f0:rt:t9:t4 0x2f8:rt:t9:t4 0x2fc:rt:t1:t5 0x300:rt:t7:t4 0x304:rt:t1:t5 0x308:rt:t0:t6 0x30c:rs:t7:t4 0x30c:rt:t7:t4 0x310:rt:t0:t6 0x314:rt:t2:t7 0x31c:rt:t2:t7 0x320:rt:t3:t9 0x328:rt:t3:t9 0x32c:rt:t4:t3 0x334:rt:t4:t3 0x338:rs:t5:t2 0x338:rt:t5:t2 0x340:rd:t6:t8 0x340:rt:t5:t2 0x344:rs:t6:t8 0x344:rt:t8:t1 0x34c:rs:t8:t1 0x34c:rt:t7:t4 0x1c0:rt:t8:t2 0x1cc:rt:t9:t8 */
/* @PostProc: pad_trailing_nop --count 2 */
#include "types.h"

typedef struct { s32 w[8]; } Mtx;
typedef struct { s16 vx, vy; } Dvec;
typedef struct { s16 vx, vy, vz, pad; } Svec;

typedef struct Spr {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    s16 clut;
    s16 x1, y1;
    u8  u1, v1;
    s16 tpage;
    s16 x2, y2;
    u8  u2, v2;
    s16 pad30;
    s16 x3, y3;
    u8  u3, v3;
    s16 pad38;
} Spr;

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
extern s16 disp_ofs_y[];
extern u8  ot_buff0[];
extern u16 taco_bill_clut;
extern Item taco_throw_tbl[];

extern Spr *prim_alloc(s32, s32);
extern void SetPolyFT4(Spr *);
extern s32  SetSemiTrans(Spr *, s32);
extern s16  GetTPage(s32, s32, s32, s32);
extern s16  GetClut(s32, s32);
extern void AddPrim(u8 *, Spr *);
extern void SetTransMatrix(Mtx *);
extern void SetRotMatrix(Mtx *);
extern void RotTransPers(Svec *, Dvec *, s32 *, s32 *);

void
taco_item_bill_disp(s32 idx, s32 lvl)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    volatile u16 pal;
    s32  sz;
    s32  flg;
    s32  i;
    s32  j;
    volatile s32 k;
    s32  xofs;
    s32  yofs;
    s32  step;
    s32  half;
    s32  h;
    s32  sel;
    s32  ot;
    Item *it;
    Spr  *s;
    Dvec *p;
    volatile u8 *q;

    pal = taco_bill_clut;
    if (lvl >= 128) {
        step = 2048;
    } else {
        step = (98304 - lvl * 512) / 256;
    }

    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    it = &taco_throw_tbl[idx];
    v.vz = it->z;
    half = step / 2;
    yofs = 0;
    k = 0;
    for (j = 0; j != 2; j++) {
        v.vy = (s16)(it->y + yofs - half);
        xofs = 0;
        p = (Dvec *)((u8 *)out + k);
        for (i = 0; i != 2; i++) {
            v.vx = (s16)(it->x + xofs - half);
            RotTransPers(&v, p, &sz, &flg);
            xofs += step;
            p += 2;
        }
        yofs += step;
        k += 4;
    }

    if (out[1][0].vx < -704) {
        return;
    }
    if (out[0][0].vx >= 705) {
        return;
    }
    ot = activeBuff;
    h = disp_ofs_y[ot];
    if (out[0][1].vy < h - 512) {
        return;
    }
    if (h + 512 < out[1][1].vy) {
        return;
    }

    sel = ((u32)it->f12 >> 2) & 1;
    s = prim_alloc(40, ot);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(80, 487);
    q = (volatile u8 *)&pal + sel;
    s->u0 = q[0];
    s->v0 = 0;
    s->u1 = (u8)(q[0] + 15);
    s->v1 = 0;
    s->u2 = q[0];
    s->v2 = 15;
    s->u3 = (u8)(q[0] + 15);
    s->v3 = 15;
    s->x0 = out[0][0].vx;
    s->y0 = out[0][0].vy;
    s->x1 = out[0][1].vx;
    s->y1 = out[0][1].vy;
    s->x2 = out[1][0].vx;
    s->y2 = out[1][0].vy;
    s->x3 = out[1][1].vx;
    s->y3 = out[1][1].vy;
    AddPrim((u8 *)((activeBuff << 10) + 16 + (s32)ot_buff0), s);
}
