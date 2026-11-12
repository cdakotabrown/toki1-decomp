/* Address: 0x8004AB84 */
/* Size: 1028 bytes (257 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=52:t9,48:t5,47:t2,46:t4,51:t7,43:t0,49:t6,50:t8,44:t3,45:t3 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x1f0:rd:t0:t1 0x1f4:rs:t0:t1 0x1f4:rd:t3:t0 0x1f8:rt:t3:t0 */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    u8 pad0[20];
    s16 ofsx[2];
    s16 ofsy[2];
} Disp;

extern s32 activeBuff;
extern Disp screen_w;
extern u32 ot_buff0[][256];
extern void SetTile();
extern void SetSemiTrans();
extern void AddPrim();

void spot_light(cx, cy, r, w, h, color, abr, pri)
s32 cx;
s32 cy;
s32 r;
s32 w;
s32 h;
u32 color;
s32 abr;
s32 pri;
{
    s32 cr;
    s32 cg;
    s32 x;
    s32 y;
    TILE *p;
    s32 red;
    s32 green;
    s32 blue;
    s32 cb;
    s32 unused;

    red = (color & 0xff0000) >> 16;
    green = (color & 0xff00) >> 8;
    blue = color & 0xff;
    w &= 0x7f;
    h &= 0x7f;
    if (w < 2) {
        w = 2;
    }
    if (h < 2) {
        h = 2;
    }
    for (x = cx - r; x < cx + r; x += w) {
        for (y = cy - r; y < cy + r; y += h) {
            if ((cx - x) * (cx - x) + (cy - y) * (cy - y) < r * r) {
                p = (TILE *)prim_alloc(16, activeBuff);
                SetTile(p);
                SetSemiTrans(p, 1);
                p->x0 = screen_w.ofsx[activeBuff] + (s16)x - w / 2;
                p->y0 = screen_w.ofsy[activeBuff] + y - h / 2;
                p->h = p->w = w;
                cr = (r * 2 - (abs(cx - x) + abs(cy - y))) * red / r;
                cg = (r * 2 - (abs(cx - x) + abs(cy - y))) * green / r;
                cb = (r * 2 - (abs(cx - x) + abs(cy - y))) * blue / r;
                if (cr > 255) {
                    cr = 255;
                }
                if (cg > 255) {
                    cg = 255;
                }
                if (cb > 255) {
                    cb = 255;
                }
                p->r0 = cr;
                p->g0 = cg;
                p->b0 = cb;
                AddPrim(&ot_buff0[activeBuff][pri], p);
            }
        }
    }
    dtd_on_tpage(0, 0, pri, abr & 3, 0);
}
