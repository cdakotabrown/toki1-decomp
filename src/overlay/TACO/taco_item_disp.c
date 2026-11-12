/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --map '128:108,132:112,136:120,140:124,144:136,146:138,148:140,152:144,154:146,156:148,158:150,160:152,162:154,164:156,166:158,168:160,170:162,172:164,176:168' */
/* @PostProc: swap_instructions --offset1 216 --offset2 220 */
/* @PostProc: swap_instructions --offset1 248 --offset2 252 */
/* @PostProc: swap_instructions --offset1 292 --offset2 296 */
/* @PostProc: swap_instructions --offset1 340 --offset2 348 */
/* @PostProc: swap_instructions --offset1 624 --offset2 628 */
/* @PostProc: swap_instructions --offset1 676 --offset2 688 */
/* @PostProc: swap_instructions --offset1 700 --offset2 712 */
/* @PostProc: swap_instructions --offset1 712 --offset2 724 */
/* @PostProc: patch_insn_reg_field 0x54:rd:t1:t0 0x60:rd:t1:t0 0x60:rs:t1:t0 0x6c:rt:t2:t1 0x78:rs:t2:t1 0x78:rt:t2:t1 0x84:rd:t1:t0 0x84:rt:t1:t0 0x90:rs:t1:t0 0x90:rt:t2:t1 0xa4:rt:t3:t2 0xac:rs:t3:t2 0xb0:rt:t8:t5 0xb4:rt:t8:t5 0xbc:rs:t3:t2 0xbc:rt:t8:t5 0xc0:rt:t7:t9 0xc4:rt:t2:t8 0xc8:rt:t1:t6 0xcc:rd:t6:t0 0xcc:rt:t7:t9 0xd0:rs:t6:t0 0xd0:rt:t9:t1 0xd4:rs:t2:t8 0xd4:rt:t5:t7 0xd8:rt:t5:t7 0xdc:rt:t9:t1 0xe4:rt:t1:t6 0x110:rt:t8:t2 0x118:rd:t7:t5 0x118:rs:t8:t2 0x11c:rs:t7:t5 0x11c:rt:t6:t8 0x120:rt:t6:t8 0x12c:rt:t1:t6 0x134:rd:t2:t9 0x134:rs:t1:t6 0x138:rs:t2:t9 0x138:rt:t5:t0 0x13c:rt:t5:t0 0x170:rt:t4:t1 0x174:rt:t3:t4 0x178:rs:t4:t1 0x180:rs:t3:t4 0x194:rd:t8:t3 0x198:rt:t8:t3 0x1a0:rt:t7:t2 0x1a4:rt:t6:t5 0x1a8:rs:t7:t2 0x1a8:rt:t6:t5 0x1b4:rt:t9:t8 0x1b8:rt:t1:t7 0x1bc:rs:t1:t7 0x1bc:rt:t9:t8 0x1c4:rt:t2:t6 0x1c8:rs:t2:t6 0x1c8:rt:t2:t6 0x1cc:rt:t5:t9 0x1d4:rs:t2:t6 0x1d4:rt:t5:t9 0x220:rt:v1:v0 0x224:rt:v1:v0 0x228:rt:v1:v0 0x22c:rt:v1:v0 0x258:rt:a3:a2 0x260:rt:a3:a2 0x268:rt:t0:a3 0x26c:rd:t3:t1 0x270:rs:t3:t1 0x270:rt:a1:a0 0x274:rs:t3:t1 0x274:rt:a0:a1 0x278:rt:a0:a1 0x27c:rt:a0:a1 0x280:rt:a1:a0 0x284:rt:a1:a0 0x288:rt:t0:a3 0x28c:rt:t0:a3 0x290:rt:a3:a2 0x294:rt:t8:t4 0x298:rt:t3:t0 0x29c:rt:t8:t4 0x2a0:rt:t7:t3 0x2a4:rd:t7:t4 0x2a8:rt:t7:t3 0x2ac:rt:t6:t2 0x2b4:rt:t6:t2 0x2b8:rt:t9:t5 0x2bc:rt:t9:t2 0x2c0:rt:t9:t5 0x2c4:rt:t1:t8 0x2c8:rs:t9:t2 0x2c8:rt:t9:t2 0x2cc:rt:t1:t8 0x2d0:rt:t2:t7 0x2d8:rt:t2:t7 0x2dc:rt:t5:t6 0x2e4:rt:t5:t6 0x2e8:rt:t4:t9 0x2f0:rt:t4:t9 0x2f4:rs:t3:t0 0x2f4:rt:t3:t0 0x2fc:rd:t8:t1 0x2fc:rt:t3:t0 0x300:rd:t6:t3 0x300:rs:t8:t1 0x300:rt:t7:t4 0x308:rs:t6:t3 0x308:rt:t9:t2 */
/* Address: 0x8013E694 */
/* Size: 812 bytes (203 instructions) */
#include "types.h"
#include "tokimeki/libgs_types.h"

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
      Svec p;
      u8  pad08[9];
      u8  f11;
      u8  f12;
      u8  pad13;
} Item;

extern Mtx GsWSMATRIX;
extern s32 activeBuff;
extern s16 disp_ofs_y[];
extern u8  ot_buff0[];
extern Item taco_item_pool[];
extern LIBGS_RVIEW2 ref_view;

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
taco_item_disp(s32 idx)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    Svec pos;
    s32  sz;
    s32  flg;
    s32  cc;
    s32  sel;
    s32  yofs;
    s32  xofs;
    s32  i;
    s32  j;
    s32  n;
    s32  m;
    s32  h;
    s32  uv;
    s32  va;
    s32  vb;
    Item *it;
    Spr  *s;
    u8   *ot;

    a = GsWSMATRIX;
    it = &taco_item_pool[idx];
    pos = it->p;
    sel = ((u32)it->f12 >> 2) & 7;
    cc = it->f11;
    pos.vz = (s16)(pos.vz - 512);
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    v.vz = pos.vz;
    yofs = 0;
    for (j = 0; j != 2; j++) {
        v.vy = (s16)(pos.vy + yofs - 64);
        xofs = 0;
        for (i = 0; i < 2; i++) {
            v.vx = (s16)(pos.vx + xofs - 64);
            RotTransPers(&v, &out[i][j], &sz, &flg);
            xofs += 128;
        }
        yofs += 128;
    }

    if (out[1][0].vx < -704) {
        return;
    }
    if (out[0][0].vx >= 705) {
        return;
    }
    h = disp_ofs_y[activeBuff];
    if (out[0][1].vy < h - 512) {
        return;
    }
    if (h + 512 < out[1][1].vy) {
        return;
    }

    n = (ref_view.vrz - pos.vz) / 127;
    m = n;
    if (n >= 256) {
        n = 255;
    } else if (m <= 0) {
        n = 1;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(1, 1, 704, 256);
    s->clut = GetClut(0, cc + 482);
    s->u0 = 97;
    uv = sel * 32;
    va = uv + 1;
    vb = uv + 31;
    s->v2 = (u8)vb;
    s->v3 = (u8)vb;
    s->v0 = (u8)va;
    s->v1 = (u8)va;
    s->u1 = 127;
    s->u3 = 127;
    s->u2 = 97;
    s->x0 = out[0][0].vx;
    s->y0 = out[0][0].vy;
    s->x1 = out[0][1].vx;
    s->y1 = out[0][1].vy;
    s->x2 = out[1][0].vx;
    s->y2 = out[1][0].vy;
    s->x3 = out[1][1].vx;
    s->y3 = out[1][1].vy;
    ot = (u8 *)((activeBuff << 10) + n * 4 + (s32)ot_buff0);
    AddPrim(ot, s);
}
