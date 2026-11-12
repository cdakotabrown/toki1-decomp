/* Address: 0x801415F8 */
/* Size: 1072 bytes (268 instructions) */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: swap_instructions --offset1 224 --offset2 232 */
/* @PostProc: swap_instructions --offset1 228 --offset2 232 */
/* @PostProc: swap_instructions --offset1 468 --offset2 476 */
/* @PostProc: swap_instructions --offset1 696 --offset2 700 */
/* @PostProc: swap_instructions --offset1 752 --offset2 756 */
/* @PostProc: swap_instructions --offset1 760 --offset2 764 */
/* @PostProc: swap_instructions --offset1 808 --offset2 820 */
/* @PostProc: swap_instructions --offset1 824 --offset2 828 */
/* @PostProc: swap_instructions --offset1 832 --offset2 836 */
/* @PostProc: swap_instructions --offset1 876 --offset2 880 */
/* @PostProc: remap_stack_slots --map '92:84,116:128,124:120,128:124,134:108,136:144,138:146,140:148,142:150,144:152,146:154,148:156,150:158,152:160,154:162,156:164,160:168' */
/* @PostProc: patch_insn_reg_field 0x1c4:rt:t8:t6 0x1c8:rt:t6:t8 0x1cc:rs:t8:t6 0x1cc:rt:t7:t9 0x1d0:rs:t6:t8 0x1d0:rt:t9:t7 0x1d4:rt:t9:t7 0x1d8:rs:t7:t9 0x1dc:rt:t7:t9 0x224:rt:t9:t8 0x228:rt:t8:t7 0x22c:rs:t8:t7 0x22c:rt:t9:t8 0x298:rd:t7:t9 0x2a0:rs:t7:t9 0x2b8:rt:v1:v0 0x2bc:rt:v0:v1 0x2cc:rt:v0:v1 0x2d0:rt:v0:v1 0x2d8:rt:v1:v0 0x2dc:rt:v1:v0 0x2f0:rt:v1:v0 0x2f4:rt:v0:v1 0x2f8:rt:a1:a0 0x2fc:rt:a0:a1 0x300:rt:a0:a1 0x304:rt:a0:a1 0x308:rt:a1:a0 0x30c:rt:a1:a0 0x310:rt:v0:v1 0x314:rt:v0:v1 0x318:rt:v1:v0 0x320:rt:v1:v0 0x328:rt:v1:v0 0x334:rt:v0:v1 0x338:rt:v1:v0 0x33c:rt:v0:v1 0x340:rt:a1:a0 0x344:rt:a0:a1 0x348:rt:a0:a1 0x34c:rt:a0:a1 0x350:rt:a1:a0 0x354:rt:a1:a0 0x358:rt:v0:v1 0x35c:rt:v0:v1 0x360:rt:v1:v0 0x368:rt:v1:v0 0x36c:rt:a1:a0 0x370:rt:a0:a1 0x374:rt:a0:a1 0x378:rt:a0:a1 0x37c:rt:a1:a0 0x380:rt:a1:a0 0x384:rt:v1:v0 0x388:rt:v1:v0 0x38c:rt:v0:v1 0x390:rt:v0:v1 0x3b8:rt:t9:t8 0x3c0:rt:t9:t8 0x3c4:rt:t8:t7 0x3cc:rt:t8:t7 0x3d4:rt:t8:t7 0x3d8:rt:t7:t9 0x3d0:rt:t7:t9 0x3e0:rs:t8:t7 0x3e0:rt:t8:t7 0x404:rd:t9:t8 0x40c:rs:t9:t8 0x40c:rt:t8:t7 */
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
extern u16 taco_splash_clut;
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
taco_splash_disp(Item *arr, s32 idx)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    volatile u16 pal;
    s32  sz;
    s32  flg;
    s32  i;
    s32  j;
    s32  n;
    s32  step;
    s32  half;
    s32  h;
    s32  f;
    s32  k;
    Item *it;
    Spr  *s;

    pal = taco_splash_clut;
    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    it = &arr[idx];
    n = (ref_view.vrz - it->z) / 127;
    if (n >= 256) {
        n = 255;
    } else if (n <= 0) {
        n = 1;
    }
    step = (n * 40 + 640) / 16;
    half = step / 2;
    v.vz = it->z;

    j = 0;
    k = 0;
    for (; j != 2; j++) {
        v.vy = (s16)(it->y + j * step - half);
        for (i = 0; i != 2; i++) {
            v.vx = (s16)(it->x + i * step - half);
            RotTransPers(&v, (Dvec *)((u8 *)&out[i][0] + k), &sz, &flg);
        }
        k += 4;
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

    f = it->f12;
    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(f % 14 * 16, 491);
    if (n < 32) {
        s->u1 = 158;
        s->u3 = 158;
        s->v2 = 62;
        s->v3 = 62;
        s->u0 = 128;
        s->v0 = 32;
        s->v1 = 32;
        s->u2 = 128;
    } else if (n < 96) {
        s->u1 = 190;
        s->u3 = 190;
        s->u0 = 160;
        s->u2 = 160;
        s->v2 = 62;
        s->v3 = 62;
        s->v0 = 32;
        s->v1 = 32;
    } else if (n < 160) {
        s->u1 = 222;
        s->u3 = 222;
        s->u0 = 192;
        s->u2 = 192;
        s->v2 = 62;
        s->v3 = 62;
        s->v0 = 32;
        s->v1 = 32;
    } else {
        s->u1 = 254;
        s->u3 = 254;
        s->u0 = 224;
        s->u2 = 224;
        s->v0 = 32;
        s->v1 = 32;
        s->v2 = 62;
        s->v3 = 62;
    }
    s->x0 = out[0][0].vx;
    s->y0 = out[0][0].vy;
    s->x1 = out[0][1].vx;
    s->y1 = out[0][1].vy;
    s->x2 = out[1][0].vx;
    s->y2 = out[1][0].vy;
    s->x3 = out[1][1].vx;
    s->y3 = out[1][1].vy;
    AddPrim((u8 *)((activeBuff << 10) + n * 4 + (s32)ot_buff0), s);
}
