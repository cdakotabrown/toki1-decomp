/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x8013D448 */
/* Size: 784 bytes (196 instructions) */
/* @PostProc: swap_instructions --offset1 264 --offset2 272 */
/* @PostProc: remap_stack_slots --map '128:112,132:116,136:128,138:130,140:132,142:134,144:136,146:138,148:140,150:142,152:144,154:146,156:148,160:152' */
/* @PostProc: patch_insn_reg_field 0x208:rt:v1:v0 0x20c:rt:v1:v0 0x210:rt:v1:v0 0x214:rt:v1:v0 0x24c:rt:a3:a2 0x254:rt:a0:a1 0x258:rt:a0:a1 0x25c:rt:a0:a1 0x268:rt:a3:a2 0x26c:rt:a3:a2 */
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
taco_spark_disp(s16 px, s16 py, s16 pz, s32 t)
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
    s32  m;
    s32  h;
    s32  uu;
    Spr  *s;
    u8   *ot;

    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    v.vz = pz;
    yofs = 0;
    for (j = 0; j != 2; j++) {
        v.vy = (s16)(py + yofs - 128);
        xofs = 0;
        for (i = 0; i < 2; i++) {
            v.vx = (s16)(px + xofs - 128);
            RotTransPers(&v, &out[i][j], &sz, &flg);
            xofs += 256;
        }
        yofs += 256;
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

    n = (ref_view.vrz - pz) / 127;
    m = n;
    if (n >= 256) {
        n = 255;
    } else if (m <= 0) {
        n = 1;
    }
    if (n >= 33) {
        n = n / 2;
    } else if (n >= 6) {
        n -= 4;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(t * 16, 484);
    uu = t * 32;
    s->u1 = (u8)(uu + 31);
    s->u3 = (u8)(uu + 31);
    s->u0 = (u8)uu;
    s->u2 = (u8)uu;
    s->v2 = 31;
    s->v3 = 31;
    s->v1 = 0;
    s->v0 = 0;
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
