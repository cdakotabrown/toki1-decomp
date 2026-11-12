/* Address: 0x8004A784 */
/* Size: 1024 bytes (256 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad2;
    u8 r3, g3, b3, p3;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad3;
} POLY_GT4;

typedef struct {
    u8 pad0[20];
    s16 ofsx[2];
    s16 ofsy[2];
} Disp;

extern s32 activeBuff;
extern Disp screen_w;
extern u32 ot_buff0[][256];

#define setRGB0(p, r, g, b) (p)->r0 = r, (p)->g0 = g, (p)->b0 = b
#define setRGB1(p, r, g, b) (p)->r1 = r, (p)->g1 = g, (p)->b1 = b
#define setRGB2(p, r, g, b) (p)->r2 = r, (p)->g2 = g, (p)->b2 = b
#define setRGB3(p, r, g, b) (p)->r3 = r, (p)->g3 = g, (p)->b3 = b
#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) (p)->u0 = _u0, (p)->v0 = _v0, (p)->u1 = _u1, (p)->v1 = _v1, (p)->u2 = _u2, (p)->v2 = _v2, (p)->u3 = _u3, (p)->v3 = _v3

void sprite_scatter_show(cx, cy, r, w, h, pri)
s32 cx;
s32 cy;
s32 r;
s32 w;
s32 h;
s32 pri;
{
    Disp *sv;
    POLY_GT4 *p;
    s32 x;
    s32 y;

    w &= 0x7f;
    h &= 0x7f;
    if (w < 8) {
        w = 8;
    }
    if (h < 8) {
        h = 8;
    }
    sv = &screen_w;
    for (x = cx - r; x < cx + r; x += w) {
        for (y = cy - r; y < cy + r; y += h) {
            if ((cx - x) * (cx - x) + (cy - y) * (cy - y) < r * r) {
                p = (POLY_GT4 *)prim_alloc(52, activeBuff);
                SetPolyGT4(p);
                SetSemiTrans(p, 0);
                p->x0 = sv->ofsx[activeBuff] + (s16)x - w / 2;
                p->y0 = sv->ofsy[activeBuff] + (s16)y - h / 2;
                p->x1 = sv->ofsx[activeBuff] + (s16)x - w / 2 + (s16)w;
                p->y1 = sv->ofsy[activeBuff] + (s16)y - h / 2;
                p->x2 = sv->ofsx[activeBuff] + (s16)x - w / 2;
                p->y2 = sv->ofsy[activeBuff] + (s16)y - h / 2 + (s16)h;
                p->x3 = sv->ofsx[activeBuff] + (s16)x - w / 2 + (s16)w;
                p->y3 = sv->ofsy[activeBuff] + (s16)y - h / 2 + (s16)h;
                setUV4(p, 192, 80, 208, 80, 192, 96, 208, 96);
                setRGB0(p, 128, 128, 128);
                setRGB1(p, 128, 128, 128);
                setRGB2(p, 128, 128, 128);
                setRGB3(p, 128, 128, 128);
                p->tpage = GetTPage(0, 0, 960, 256);
                p->clut = GetClut(sv->ofsx[activeBuff] + (rand() & 8) + (s16)x,
                                        sv->ofsy[activeBuff] + ((rand() & 8) + (s16)y));
                AddPrim(&ot_buff0[activeBuff][pri], p);
            }
        }
    }
}
