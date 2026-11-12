/* en_nichi_grid_quad_disp - EN_NICHI overlay, Address: 0x80136138, Size: 744 bytes */
/* @SymbolStart: 0x80136138 */
/* @SymbolSize: 744 */
/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --map 96:84,88:100,84:108,76:96,80:88,72:104,68:112 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0x288 --offset2 0x294 */
/* @PostProc: swap_instructions --offset1 0x28c --offset2 0x290 */
/* @PostProc: patch_insn_reg_field 0xcc:rd:a1:t0 0xd8:rt:s5:s4 0xfc:rt:s6:s5 0x100:rs:a1:t0 0x104:rs:a1:t0 0x108:rs:s5:s4 0x11c:rs:s5:s4 0x120:rs:s6:s5 0x128:rd:s4:s6 0x128:rs:fp:t2 0x130:rs:t2:fp 0x13c:rs:s5:s3 0x13c:rt:s5:s3 0x140:rs:s6:s4 0x140:rt:s6:s4 0x148:rs:s3:s5 0x148:rt:s3:s5 0x160:rs:s7:s6 0x160:rt:s7:s6 0x164:rs:s4:s7 0x164:rt:s4:s7 0x260:rs:s4:s7 0x260:rt:t7:t8 0x270:rt:t8:t3 0x27c:rs:s6:s5 0x280:rt:t3:t7 0x288:rt:t3:t7 0x290:rt:t8:t3 0x294:rt:t7:t8 0x298:rt:s5:s4 0x29c:rt:s7:s6 */
#include "types.h"

typedef struct {
    u8 tag[4];
    u8 r;
    u8 g;
    u8 b;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    s16 clut;
    s16 x1;
    s16 y1;
    u8 u1;
    u8 v1;
    s16 tpage;
    s16 x2;
    s16 y2;
    u8 u2;
    u8 v2;
    u8 pad1E[2];
    s16 x3;
    s16 y3;
    u8 u3;
    u8 v3;
    u8 pad26[2];
} EnNichiQuad;

extern s16 en_nichi_grid_buf[][12][4];
extern s32 en_nichi_sway_x;
extern s32 en_nichi_sway_y;
extern volatile s32 en_nichi_grid_bright;
extern s32 activeBuff;
extern u8 ot_buff0[];

extern EnNichiQuad *prim_alloc(s32, s32);
extern void SetPolyFT4(EnNichiQuad *);
extern void SetShadeTex(EnNichiQuad *, s32);
extern void SetSemiTrans(EnNichiQuad *, s32);
extern s32 GetClut(s32, s32);
extern s32 GetTPage(s32, s32, s32, s32);
extern void AddPrim(u8 *, EnNichiQuad *);

void en_nichi_grid_quad_disp(s32 gx, s32 gy)
{
    EnNichiQuad *p;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 y0;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 ox;
    s32 oy;
    s32 u0;
    s32 u1;
    s32 u2;
    s32 u3;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 off;

    x0 = en_nichi_grid_buf[gx][gy][1] + gx * 25 - 25;
    x1 = en_nichi_grid_buf[gx + 1][gy][1] + gx * 25 - 1;
    x2 = en_nichi_grid_buf[gx][gy + 1][1] + gx * 25 - 25;
    x3 = en_nichi_grid_buf[gx + 1][gy + 1][1] + gx * 25 - 1;
    y0 = en_nichi_grid_buf[gx][gy][3] + gy * 25 - 25;
    y1 = en_nichi_grid_buf[gx + 1][gy][3] + gy * 25 - 25;
    ox = gx * 25 + en_nichi_sway_x;
    oy = gy * 25 + en_nichi_sway_y;
    y2 = en_nichi_grid_buf[gx][gy + 1][3] + gy * 25 - 1;
    y3 = en_nichi_grid_buf[gx + 1][gy + 1][3] + gy * 25 - 1;

    u2 = ox + 39;
    u3 = ox + 63;
    v0 = oy + 39;
    v2 = oy + 63;
    u0 = u2;
    u1 = u3;
    v1 = v0;
    v3 = v2;

    while (u2 >= 193) {
        u0 -= 128;
        u1 -= 128;
        u2 -= 128;
        u3 -= 128;
    }
    while (v0 >= 193) {
        v1 -= 128;
        v2 -= 128;
        v3 -= 128;
        v0 -= 128;
    }

    p = prim_alloc(40, activeBuff);
    SetPolyFT4(p);
    SetShadeTex(p, 0);
    SetSemiTrans(p, 1);
    p->r = (u8)en_nichi_grid_bright;
    p->g = (u8)en_nichi_grid_bright;
    p->b = (u8)en_nichi_grid_bright;
    p->clut = (s16)GetClut(0, 511);
    p->tpage = (s16)GetTPage(1, 4, 448, 0);

    off = -activeBuff * 240 + 240;
    p->x0 = (s16)x0;
    p->y0 = (s16)(y0 + off);
    p->x1 = (s16)(x1 + 1);
    p->y1 = (s16)(y1 + off);
    p->x2 = (s16)x2;
    p->y2 = (s16)(y2 + off + 1);
    p->x3 = (s16)(x3 + 1);
    p->y3 = (s16)(y3 + off + 1);
    p->v2 = (u8)(v2 + 1);
    p->v3 = (u8)(v3 + 1);
    p->u3 = (u8)(u1 + 1);
    p->u1 = (u8)(u3 + 1);
    p->u2 = (u8)u0;
    p->v1 = (u8)v1;
    p->v0 = (u8)v0;
    p->u0 = (u8)u2;

    AddPrim(&ot_buff0[activeBuff * 1024 + 20], p);
}
