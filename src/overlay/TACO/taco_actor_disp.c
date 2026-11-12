/* @PostProc: patch_stack_complete --old 184 --new 200 */
/* @PostProc: remap_stack_slots --map '116:100,120:112,124:116,128:136,130:138,132:140,134:142,136:144,138:146,140:148,142:150,144:152,146:154,148:156,152:160' */
/* @PostProc: swap_instructions --offset1 244 --offset2 248 */
/* @PostProc: swap_instructions --offset1 292 --offset2 300 */
/* @PostProc: pad_trailing_nop --count 1 */
/* @PostProc: patch_insn_reg_field 0x19c:rt:t0:t4 0x1a0:rs:t0:t4 0x1a0:rt:t0:t4 0x1a4:rt:t1:t5 0x1ac:rs:t0:t4 0x1ac:rt:t1:t5 0x1bc:rd:t3:t0 0x1c0:rs:t3:t0 0x1c0:rt:t4:t1 0x1c4:rd:t5:t2 0x1c4:rt:t4:t1 0x1c8:rt:t5:t2 0x24c:rt:a0:a1 0x254:rt:a0:a1 0x258:rt:a0:a1 */
/* Address: 0x801409EC */
/* Size: 780 bytes (195 instructions) + 8 bytes of alignment nops */
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
taco_actor_disp(Item *arr, s32 idx)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    s32  sz;
    s32  flg;
    s32  yofs;
    s32  xofs;
    s32  i;
    s32  j;
    s32  n;
    s32  h;
    s16  k;
    Item *it;
    Spr  *s;

    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    it = &arr[idx];
    v.vz = it->z;
    yofs = 0;
    for (j = 0; j != 2; j++) {
        v.vy = (s16)(it->y + yofs - 16);
        xofs = 0;
        for (i = 0; i < 2; i++) {
            v.vx = (s16)(it->x + xofs - 16);
            RotTransPers(&v, &out[i][j], &sz, &flg);
            xofs += 32;
        }
        yofs += 32;
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
    k = (s16)(((u32)it->f12 >> 4) & 1);
    if (n >= 256) {
        n = 255;
    } else if (n <= 0) {
        return;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 0);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(1, 1, 960, 256);
    s->clut = GetClut(256, 504);
    s->u1 = (u8)(k * 32 + 32);
    s->u3 = (u8)(k * 32 + 32);
    s->v0 = 96;
    s->v1 = 96;
    s->u0 = (u8)(k * 32);
    s->u2 = (u8)(k * 32);
    s->v2 = 128;
    s->v3 = 128;
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
