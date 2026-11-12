/* @SymbolSize: 1948 */
/* @CompileFlags: -cckr */
/* @PostProc: remap_stack_slots --map 100:88 --at 0x5E8,0x5F4,0x658,0x668 --keep-frame */
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
    u16 parow;
    s16 x3, y3;
    u8  u3, v3;
    u16 pad2;
} PolyFT4;

typedef struct {
    Svec p;
    u8 papal8[9];
    u8 f11;
    u8 f12;
    u8 parow3;
} Item;

#define setRGB0(p, r, g, b) (p)->r0 = (r), (p)->g0 = (g), (p)->b0 = (b)
#define setUV4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->u0 = (a0), (p)->v0 = (b0), (p)->u1 = (a1), (p)->v1 = (b1), (p)->u2 = (a2), (p)->v2 = (b2), (p)->u3 = (a3), (p)->v3 = (b3)
#define setXY4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->x0 = (a0), (p)->y0 = (b0), (p)->x1 = (a1), (p)->y1 = (b1), (p)->x2 = (a2), (p)->y2 = (b2), (p)->x3 = (a3), (p)->y3 = (b3)

extern Mtx  GsWSMATRIX;
extern Item taco_item_pool[];
extern s32  activeBuff;
extern s16  disp_ofs_y[];
extern u8   taco_item_pal_tbl[];
extern u8   taco_item_row_tbl[];
extern u8   taco_item_u_tbl[];
extern u8   taco_item_v_tbl[];
extern u32  ot_buff0[][256];
extern LIBGS_RVIEW2 ref_view;

extern PolyFT4 *prim_alloc();
extern void SetPolyFT4();
extern void SetSemiTrans();
extern u16  GetTPage();
extern u16  GetClut();
extern void AddPrim();
extern s32  rsin();
extern s32  rcos();
extern void SetTransMatrix();
extern void SetRotMatrix();
extern void RotTransPers();

void taco_item_disp2(idx)
s32 idx;
{
    s32 i, j;
    Mtx a;
    Mtx b;
    Svec v;
    Dvec out[2][2];
    Svec pos;
    Item *it;
    PolyFT4 *s;
    s32 n;
    s32 pal;
    s32 sz;
    s32 flg;
    s32 row;
    s32 cc;
    s32 h;
    s32 sc;
    s32 pad1;
    s32 pad2;
    s32 sa;
    s32 pad3;

    a = GsWSMATRIX;
    it = &taco_item_pool[idx];
    pos = it->p;
    pos.vz -= 512;
    cc = it->f11 & 7;
    h = it->f12 >> 1;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    v.vz = pos.vz;
    for (j = 0; j < 2; j++) {
        v.vy = pos.vy + j * 128 - 64;
        for (i = 0; i < 2; i++) {
            v.vx = pos.vx + i * 128 - 64;
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
        n = 1;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 0);
    setRGB0(s, 128, 128, 128);
    s->tpage = GetTPage(0, 1, 896, 256);
    pal = taco_item_pal_tbl[cc] ? h % taco_item_pal_tbl[cc] : 0;
    row = taco_item_row_tbl[cc];
    s->clut = GetClut(pal << 4, row + 480);
    setUV4(s, taco_item_u_tbl[cc] + 1, taco_item_v_tbl[cc] + 1, taco_item_u_tbl[cc] + 15, taco_item_v_tbl[cc] + 1, taco_item_u_tbl[cc] + 1, taco_item_v_tbl[cc] + 15, taco_item_u_tbl[cc] + 15, taco_item_v_tbl[cc] + 15);
    setXY4(s, out[1][0].vx, out[1][0].vy, out[0][0].vx, out[0][0].vy, out[1][1].vx, out[1][1].vy, out[0][1].vx, out[0][1].vy);
    AddPrim(&ot_buff0[activeBuff][n], s);

    sa = rsin(it->f12 << 4) * 128 / 4096;
    v.vz = pos.vz;
    for (j = 0; j < 2; j++) {
        v.vy = pos.vy + j * sa * 2 - sa;
        for (i = 0; i < 2; i++) {
            v.vx = pos.vx + i * 256 - 128;
            RotTransPers(&v, &out[i][j], &sz, &flg);
        }
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    setRGB0(s, 128, 128, 128);
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(h % 14 << 4, 487);
    setUV4(s, 97, 33, 127, 33, 97, 63, 127, 63);
    setXY4(s, out[1][0].vx, out[1][0].vy, out[0][0].vx, out[0][0].vy, out[1][1].vx, out[1][1].vy, out[0][1].vx, out[0][1].vy);
    AddPrim(&ot_buff0[activeBuff][n], s);

    sc = rcos(it->f12 << 4) * 128 / 4096;
    v.vz = pos.vz;
    for (j = 0; j < 2; j++) {
        v.vy = pos.vy + j * 256 - 128;
        for (i = 0; i < 2; i++) {
            v.vx = pos.vx + i * sc * 2 - sc;
            RotTransPers(&v, &out[i][j], &sz, &flg);
        }
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    setRGB0(s, 128, 128, 128);
    s->tpage = GetTPage(0, 1, 896, 256);
    s->clut = GetClut(h % 14 << 4, 487);
    setUV4(s, 97, 33, 127, 33, 97, 63, 127, 63);
    setXY4(s, out[1][0].vx, out[1][0].vy, out[0][0].vx, out[0][0].vy, out[1][1].vx, out[1][1].vy, out[0][1].vx, out[0][1].vy);
    AddPrim(&ot_buff0[activeBuff][n], s);
}
