/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-100 IDO52_COPYPROP_SITE=lod:3:deny IDO52_SPLIT_BLOCKS=4#1:2+3+5+4+6+7 IDO52_FORCE_WEB_COLOR=28:v1,24:s7,133:t0 IDO52_AS1_TIE_TAKE=311 IDO52_AS1_TIE_REJECT=245,246,247 IDO52_UGEN_DEST_TEMP=55,56 */
/* @SymbolSize: 832 */
/* Address: 0x8013D758 */
/* Size: 832 bytes (208 instructions; the .sym's 840 covers two trailing */
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

extern prim_alloc();
extern void SetPolyFT4();
extern void SetSemiTrans();
extern GetTPage();
extern GetClut();
extern void AddPrim();
extern void SetTransMatrix();
extern void SetRotMatrix();
extern void RotTransPers();

taco_spark_disp2(px, py, pz, t)
short px, py, pz;
{
    s32 n;
    Spr *s;
    Mtx a;
    Svec v;
    Dvec out[2][2];
    Dvec *q;
    s32 j;
    s32 sz;
    s32 flg;
    s32 i;
    s32 xx;
    s32 yy;
    s32 step;
    s32 half;
    s32 off;
    s32 ix;
    s32 h;
    s32 uu;
    s32 vv;

    step = 256;
    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);
    yy = py;
    if (t < 0 || t == 6) {
        step = 512;
    }
    v.vz = pz;
    j = 0;
    half = step / 2;
    off = 0;
    do {
        v.vy = yy - half;
        i = 0;
        xx = px;
        q = (Dvec *)((u8 *)out + off);
        do {
            v.vx = xx - half;
            RotTransPers(&v, q, &sz, &flg);
            i++;
            xx += step;
            q += 2;
        } while (i != 2);
        j++;
        yy += step;
        off += 4;
    } while (j != 2);
    if (out[1][0].vx < -704) {
        return;
    }
    if (out[0][0].vx >= 705) {
        return;
    }
    ix = activeBuff;
    h = disp_ofs_y[ix];
    if (out[0][1].vy < h - 512) {
        return;
    }
    if (h + 512 < out[1][1].vy) {
        return;
    }
    n = (ref_view.vrz - pz) / 127;
    if (n >= 256) {
        n = 255;
    } else if (n <= 0) {
        n = 1;
    }
    s = prim_alloc(40, ix);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(0, 485);
    uu = t * 64;
    s->u0 = uu;
    vv = ((t & 4) << 4) + 128;
    s->v2 = vv + 63;
    s->v3 = vv + 63;
    s->v0 = vv;
    s->v1 = vv;
    s->u1 = uu + 63;
    s->u3 = uu + 63;
    s->u2 = uu;
    s->x0 = out[0][0].vx;
    s->y0 = out[0][0].vy;
    s->x1 = out[0][1].vx;
    s->y1 = out[0][1].vy;
    s->x2 = out[1][0].vx;
    s->y2 = out[1][0].vy;
    s->x3 = out[1][1].vx;
    s->y3 = out[1][1].vy;
    AddPrim(ot_buff0 + (activeBuff << 10) + n * 4, s);
}
