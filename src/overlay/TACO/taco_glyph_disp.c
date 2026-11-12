/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_ADJSAVE=37:1.1 */
#include "types.h"

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

typedef struct { u8 u, v; } UV;

extern s32 activeBuff;
#define setUV4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->u0 = (a0), (p)->v0 = (b0), (p)->u1 = (a1), (p)->v1 = (b1), (p)->u2 = (a2), (p)->v2 = (b2), (p)->u3 = (a3), (p)->v3 = (b3)
#define setXY4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->x0 = (a0), (p)->y0 = (b0), (p)->x1 = (a1), (p)->y1 = (b1), (p)->x2 = (a2), (p)->y2 = (b2), (p)->x3 = (a3), (p)->y3 = (b3)
#define SX(v) (screen_w[activeBuff + 10] + (v))
#define SY(v) (screen_w[activeBuff + 12] + (v))
extern s16 screen_w[];
extern u32 ot_buff0[][256];
extern UV taco_glyph_uv_tbl[];
extern UV taco_glyph_uv_tbl_10[];
extern UV taco_glyph_uv_tbl_36;
extern UV taco_glyph_uv_tbl_37;
extern UV taco_glyph_uv_tbl_38;
extern UV taco_glyph_uv_tbl_39;

extern PolyFT4 *prim_alloc();
extern void SetPolyFT4();
extern void SetShadeTex();
extern void SetSemiTrans();
extern u16 GetTPage();
extern u16 GetClut();
extern void AddPrim();
extern void dtd_on_tpage();

taco_glyph_disp(c, pal, pri, col, dx, dy, sw, sh, mode)
u8 c;
s32 pal;
s32 pri;
s32 col;
s32 dx;
s32 dy;
s32 sw;
s32 sh;
s32 mode;
{
    PolyFT4 *s;
    u8 i;
    s32 q;
    s32 x0, y0, x1, y1, x2, y2, x3, y3;
    s32 w;

    if (mode == 3) {
        w = 12;
    } else {
        w = 11;
    }
    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetShadeTex(s, 0);
    SetSemiTrans(s, 0);
    s->b0 = col;
    s->g0 = col;
    s->r0 = col;

    q = w * sw / 8192;
    x0 = SX(dx - q);
    x3 = SX(dx + q);
    x1 = SX(dx + q);
    x2 = SX(dx - q);
    if (mode != 1) {
        q = sh * 12 / 8192;
        y0 = SY(dy - q);
        y1 = SY(dy - q);
        y2 = SY(dy + q);
        y3 = SY(dy + q);
    } else {
        q = sh * 12 / 4096;
        y0 = SY(dy - q);
        y1 = SY(dy - q);
        y2 = SY(dy);
        y3 = SY(dy);
    }
    s->tpage = GetTPage(0, 0, 832, 384);
    setXY4(s, x0, y0, x1, y1, x2, y2, x3, y3);

    if (c >= '0' && c <= '9') {
        i = c - '0';
        setUV4(s, taco_glyph_uv_tbl[i].u + 128, taco_glyph_uv_tbl[i].v, taco_glyph_uv_tbl[i].u + w + 128, taco_glyph_uv_tbl[i].v, taco_glyph_uv_tbl[i].u + 128, taco_glyph_uv_tbl[i].v + 12, taco_glyph_uv_tbl[i].u + w + 128, taco_glyph_uv_tbl[i].v + 12);
    } else if ((u8)(i = c - 'A') < 26) {
        setUV4(s, taco_glyph_uv_tbl_10[i].u + 128, taco_glyph_uv_tbl_10[i].v, taco_glyph_uv_tbl_10[i].u + w + 128, taco_glyph_uv_tbl_10[i].v, taco_glyph_uv_tbl_10[i].u + 128, taco_glyph_uv_tbl_10[i].v + 12, taco_glyph_uv_tbl_10[i].u + w + 128, taco_glyph_uv_tbl_10[i].v + 12);
    } else if (c == '.') {
        setUV4(s, taco_glyph_uv_tbl_36.u + 128, taco_glyph_uv_tbl_36.v, taco_glyph_uv_tbl_36.u + w + 128, taco_glyph_uv_tbl_36.v, taco_glyph_uv_tbl_36.u + 128, taco_glyph_uv_tbl_36.v + 12, taco_glyph_uv_tbl_36.u + w + 128, taco_glyph_uv_tbl_36.v + 12);
    } else if (c == '-') {
        setUV4(s, taco_glyph_uv_tbl_37.u + 128, taco_glyph_uv_tbl_37.v, taco_glyph_uv_tbl_37.u + w + 128, taco_glyph_uv_tbl_37.v, taco_glyph_uv_tbl_37.u + 128, taco_glyph_uv_tbl_37.v + 12, taco_glyph_uv_tbl_37.u + w + 128, taco_glyph_uv_tbl_37.v + 12);
    } else if (c == '!') {
        setUV4(s, taco_glyph_uv_tbl_39.u + 128, taco_glyph_uv_tbl_39.v, taco_glyph_uv_tbl_39.u + w + 128, taco_glyph_uv_tbl_39.v, taco_glyph_uv_tbl_39.u + 128, taco_glyph_uv_tbl_39.v + 12, taco_glyph_uv_tbl_39.u + w + 128, taco_glyph_uv_tbl_39.v + 12);
    } else if (c == '@') {
        setUV4(s, 166, 211, w + 166, 211, 166, 223, w + 166, 223);
    } else if (c == '*') {
        setUV4(s, 190, 211, w + 190, 211, 190, 223, w + 190, 223);
    } else if (c == '#') {
        setUV4(s, 238, 211, w + 238, 211, 238, 223, w + 238, 223);
    } else {
        setUV4(s, taco_glyph_uv_tbl_38.u + 128, taco_glyph_uv_tbl_38.v, taco_glyph_uv_tbl_38.u + w + 128, taco_glyph_uv_tbl_38.v, taco_glyph_uv_tbl_38.u + 128, taco_glyph_uv_tbl_38.v + 12, taco_glyph_uv_tbl_38.u + w + 128, taco_glyph_uv_tbl_38.v + 12);
    }
    s->clut = GetClut(pal << 4, 495);
    AddPrim(&ot_buff0[activeBuff][pri], s);
    dtd_on_tpage(832, 384, pri, 0, 0);
}
