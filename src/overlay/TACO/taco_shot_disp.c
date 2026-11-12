/* @SymbolSize: 1336 */
/* @RodataBase: 0x8015DA30 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/libgs_types.h"

typedef struct { s32 m[8]; } Mtx;
typedef struct { s16 vx, vy; } Dvec;
typedef struct { s16 vx, vy, vz, pad; } Svec;

typedef struct {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    u16 clut;
    s16 x1, y1;
    u8  u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8  u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8  u3, v3;
    u16 pad2;
} PolyFT4;

typedef struct {
    Svec p;
    u8 pad8[9];
    u8 f11;
    u8 f12;
    u8 pad13;
} Item;

#define setRGB0(p, r, g, b) (p)->r0 = (r), (p)->g0 = (g), (p)->b0 = (b)
#define setUV4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->u0 = (a0), (p)->v0 = (b0), (p)->u1 = (a1), (p)->v1 = (b1), (p)->u2 = (a2), (p)->v2 = (b2), (p)->u3 = (a3), (p)->v3 = (b3)
#define setXY4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->x0 = (a0), (p)->y0 = (b0), (p)->x1 = (a1), (p)->y1 = (b1), (p)->x2 = (a2), (p)->y2 = (b2), (p)->x3 = (a3), (p)->y3 = (b3)

extern Mtx  GsWSMATRIX;
extern Item taco_pickup_tbl[];
extern s32  activeBuff;
extern s16  disp_ofs_y[];
extern u32  ot_buff0[][256];
extern LIBGS_RVIEW2 ref_view;

extern PolyFT4 *prim_alloc();
extern void SetPolyFT4();
extern void SetSemiTrans();
extern u16  GetTPage();
extern u16  GetClut();
extern void AddPrim();
extern void SetTransMatrix();
extern void SetRotMatrix();
extern void RotTransPers();

taco_shot_disp(idx)
s32 idx;
{
    PolyFT4 *s;
    s32 step;
    Mtx a;
    Svec v;
    Dvec out[2][2];
    s32 i, j;
    s32 sz;
    s32 flg;
    s32 n;
    s32 sel;
    Item *it;
    s32 kind;
    s32 c;
    Svec pos;

    it = &taco_pickup_tbl[idx];
    pos = it->p;
    sel = it->f11 & 0xF;
    a = GsWSMATRIX;
    pos.vz -= 512;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    switch (sel) {
    case 3:
    case 5:
        step = 256;
        break;
    case 0:
        step = 64;
        break;
    case 1:
    case 2:
    case 4:
    default:
        step = 128;
        break;
    }

    v.vz = pos.vz;
    kind = sel;
    for (j = 0; j < 2; j++) {
        v.vy = pos.vy + j * step - step / 2;
        for (i = 0; i < 2; i++) {
            v.vx = pos.vx + i * step - step / 2;
            RotTransPers(&v, &out[i][j], &sz, &flg);
        }
    }

    if (out[1][0].vx < -704) return;
    if (out[0][0].vx > 704) return;
    if (out[0][1].vy < disp_ofs_y[activeBuff] - 512) return;
    if (out[1][1].vy > disp_ofs_y[activeBuff] + 512) return;

    n = (ref_view.vrz - pos.vz) / 127;
    if (n > 255) {
        n = 255;
    } else if (n < 1) {
        return -1;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    switch (sel) {
    case 5:
        c = 255;
        break;
    case 6:
        c = 255;
        break;
    default:
        c = 128;
        break;
    }
    setRGB0(s, c, c, c);
    switch (kind) {
    case 3:
    case 5:
    case 6:
        SetSemiTrans(s, 1);
        break;
    case 0:
    case 1:
    case 2:
    case 4:
    default:
        SetSemiTrans(s, 0);
        break;
    }
    if (sel != 4) {
        s->tpage = GetTPage(1, 1, 960, 256);
    } else {
        s->tpage = GetTPage(0, 1, 896, 256);
    }
    switch (sel) {
    case 5:
        s->clut = GetClut(0, 490);
        break;
    default:
        s->clut = GetClut(256, 504);
        break;
    case 4:
        s->clut = GetClut(0, 485);
        break;
    }
    switch (kind) {
    case 0:
    case 1:
    default:
        setUV4(s, 1, 65, 31, 65, 1, 95, 31, 95);
        break;
    case 2:
        setUV4(s, 65, 1, 127, 1, 65, 63, 127, 63);
        break;
    case 3:
        setUV4(s, 1, 1, 63, 1, 1, 63, 63, 63);
        break;
    case 4:
        setUV4(s, 64, 128, 127, 128, 64, 191, 127, 191);
        break;
    }
    setXY4(s, out[1][0].vx, out[1][0].vy, out[0][0].vx, out[0][0].vy, out[1][1].vx, out[1][1].vy, out[0][1].vx, out[0][1].vy);
    AddPrim(&ot_buff0[activeBuff][n], s);
}
