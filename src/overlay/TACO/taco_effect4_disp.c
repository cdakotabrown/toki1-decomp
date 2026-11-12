/* @PostProc: patch_stack_complete --old 184 --new 208 */
/* @PostProc: remap_stack_slots --map '112:96,118:108,128:144,130:146,132:148,134:150,136:152,138:154,140:156,142:158,144:160,146:162,148:164,152:168' */
/* @PostProc: swap_instructions --offset1 256 --offset2 260 */
/* @PostProc: swap_instructions --offset1 304 --offset2 312 */
/* @PostProc: patch_insn_reg_field 0x60:rd:s3:s2 0xc4:rt:s3:s2 0x1a8:rt:t1:t3 0x1ac:rs:t1:t3 0x1ac:rt:t1:t3 0x1b0:rt:t2:t4 0x1b8:rs:t1:t3 0x1b8:rt:t2:t4 0x1c8:rd:t4:t1 0x1cc:rs:t4:t1 0x218:rt:v1:v0 0x21c:rt:v1:v0 0x220:rt:v1:v0 0x224:rt:v1:v0 0x25c:rt:a2:a0 0x260:rt:a2:a0 0x26c:rt:a2:a0 0x27c:rt:a3:a1 0x280:rt:a3:a1 0x28c:rt:a3:a1 */
/* Address: 0x801412C8 */
/* Size: 816 bytes (204 instructions) */
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
extern u16 taco_effect4_clut;
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
taco_effect4_disp(Item *arr, s32 idx)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    s32  sz;
    s32  flg;
    volatile u16 pal;
    s32  yofs;
    s32  xofs;
    s32  i;
    s32  j;
    s32  n;
    s32  h;
    Item *it;
    Spr  *s;
    volatile u8 *q;

    pal = taco_effect4_clut;
    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    it = &arr[idx];
    v.vz = it->z;
    yofs = 0;
    for (j = 0; j != 2; j++) {
        v.vy = (s16)(it->y + yofs - 40);
        xofs = 0;
        for (i = 0; i < 2; i++) {
            v.vx = (s16)(it->x + xofs - 40);
            RotTransPers(&v, &out[i][j], &sz, &flg);
            xofs += 80;
        }
        yofs += 80;
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

    n = (ref_view.vrz - it->z) / 127;
    j = ((u32)it->f12 >> 2) & 1;
    if (n >= 256) {
        n = 255;
    } else if (n <= 0) {
        n = 1;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(128, 484);
    q = (volatile u8 *)&pal + j;
    s->u0 = q[0];
    s->v0 = 1;
    s->u1 = (u8)(q[0] + 14);
    s->v1 = 1;
    s->u2 = q[0];
    s->v2 = 15;
    s->u3 = (u8)(q[0] + 14);
    s->v3 = 15;
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
