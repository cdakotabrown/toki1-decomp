/* Address: 0x8008612C */
/* Size: 760 bytes (190 instructions) */
/* Region: Library band (IDO 5.2, as1-assembled: the moves are `or`) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:t8,15:t0,16:t1,17:t2,18:t3,19:t4,20:t6,21:t5,22:t7 IDO52_FORCE_WEB_COLOR=36:s7,39:s8 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --keep-frame --map '64:80,68:96,88:104,104:128,120:144' */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c4 */
#include "types.h"

typedef struct {
    u32 tag;
    u8  r, g, b;
    u8  code;
    s16 x0, y0;
    u8  u0, v0;
    u16 clut;
    s16 x1, y1;
    u8  u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8  u2, v2;
    u16 pad;
    s16 x3, y3;
    u8  u3, v3;
    u16 pad2;
} PolyFT4;

extern s32 activeBuff;
extern s16 screen_w[];
extern u8 screen_bri;
extern u8 ot_buff0[];

extern s32 *prim_alloc(s32, s32);
extern void SetPolyFT4(u32 *);
extern void SetSemiTrans(u8 *, s32);
extern void SetShadeTex(u8 *, s32);
extern s32 GetTPage(s32, s32, s32, s32);
extern s32 GetClut(s32, s32);
extern void AddPrim(u32 *, u32 *);

void scroll_poly_disp(u32 tx, s32 slot, s16 ox, s16 oy, s16 wrap)
{
    PolyFT4 *p;
    s32 i;
    s32 u;

    for (i = 0; i < 5; i++) {
        p = (PolyFT4 *)prim_alloc(40, activeBuff);
        SetPolyFT4((u32 *)p);
        SetSemiTrans((u8 *)p, 0);
        SetShadeTex((u8 *)p, 0);
        p->r = screen_bri;
        p->g = screen_bri;
        p->b = screen_bri;
        u = (tx * 2 + (i % 2) * 64) & 0x7F;
        p->u1 = u + 64;
        p->u3 = u + 64;
        p->u0 = u;
        p->u2 = u;
        p->v3 = ox + oy;
        p->v2 = ox + oy;
        p->v1 = ox;
        p->v0 = ox;
        p->x0 = screen_w[activeBuff + 10] + i * 64 - 160;
        p->y0 = screen_w[activeBuff + 12] + ox - 120;
        p->x1 = screen_w[activeBuff + 10] + i * 64 - 96;
        p->y1 = screen_w[activeBuff + 12] + ox - 120;
        p->x2 = screen_w[activeBuff + 10] + i * 64 - 160;
        p->y2 = screen_w[activeBuff + 12] + ox + oy - 120;
        p->x3 = screen_w[activeBuff + 10] + i * 64 - 96;
        p->y3 = screen_w[activeBuff + 12] + ox + oy - 120;
        p->tpage = GetTPage(1, 0, (((u32)(i * 32 + tx) >> 6) << 6) % (u32)wrap + 320, 0);
        p->clut = GetClut(0, 496);
        AddPrim((u32 *)(ot_buff0 + activeBuff * 1024 + slot * 4), (u32 *)p);
    }
}
