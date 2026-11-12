/* Address: 0x80058714 */
/* Size: 2444 bytes (611 instructions); the last 3 words are alignment nops, */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80058714 */
/* @SymbolSize: 2432 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void *prim_alloc(s32 size, s32 a1);
extern void SetSprt(void *a0);
extern void SetSemiTrans(void *a0, s32 a1);
extern void SetShadeTex(void *a0, s32 a1);
extern void AddPrim(void *a0, void *a1);
extern void goto_tpage(s32, s32, s32, s32, s32);

extern s32 activeBuff;
extern u8 bg_change_tbl2;
extern u8 bg_change_tbl0;
extern u8 bg_change_tbl1;
extern s16 screen_w[];
extern u8 ot_buff0[];

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 w, h;
} SPRT;

void bg_change(s32 arg0) {
    s32 unused0, unused1;
    SPRT *sp;
    u8 *p;

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    SetSemiTrans(sp, 1);
    SetShadeTex(sp, 0);

    sp->r0 = 128 - bg_change_tbl2;
    sp->g0 = 128 - bg_change_tbl2;
    sp->b0 = 128 - bg_change_tbl2;

    if (arg0 == 1) {
        sp->x0 = screen_w[activeBuff + 10] - 160;
        sp->y0 = screen_w[activeBuff + 12] - 120;
        sp->v0 = 0;
        sp->u0 = 0;
        sp->w = 192; sp->h = 160;
    } else {
        sp->x0 = screen_w[activeBuff + 10] - 160;
        sp->y0 = screen_w[activeBuff + 12] - 48;
        sp->v0 = 160;
        sp->u0 = 0;
        sp->w = 192; sp->h = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 40], sp);

    p = &bg_change_tbl0 + arg0;

    if (*p == 0) {
        goto_tpage(320, 256, 10, 1, 2);
    } else {
        goto_tpage(704, 0, 10, 1, 2);
    }

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    SetSemiTrans(sp, 1);
    SetShadeTex(sp, 0);

    sp->r0 = 128 - bg_change_tbl2;
    sp->g0 = 128 - bg_change_tbl2;
    sp->b0 = 128 - bg_change_tbl2;

    if (p == &bg_change_tbl1) {
        sp->x0 = screen_w[activeBuff + 10] + 32;
        sp->y0 = screen_w[activeBuff + 12] - 120;
        sp->v0 = 0;
        sp->u0 = 0;
        sp->w = 128; sp->h = 160;
    } else {
        sp->x0 = screen_w[activeBuff + 10] + 32;
        sp->y0 = screen_w[activeBuff + 12] - 48;
        sp->v0 = 160;
        sp->u0 = 0;
        sp->w = 128; sp->h = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 40], sp);

    if (*p == 0) {
        goto_tpage(512, 256, 10, 1, 2);
    } else {
        goto_tpage(896, 0, 10, 1, 2);
    }

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    SetSemiTrans(sp, 0);
    SetShadeTex(sp, 0);

    sp->r0 = bg_change_tbl2;
    sp->g0 = bg_change_tbl2;
    sp->b0 = bg_change_tbl2;

    if (p == &bg_change_tbl1) {
        sp->x0 = screen_w[activeBuff + 10] - 160;
        sp->y0 = screen_w[activeBuff + 12] - 120;
        sp->v0 = 0;
        sp->u0 = 0;
        sp->w = 192; sp->h = 160;
    } else {
        sp->x0 = screen_w[activeBuff + 10] - 160;
        sp->y0 = screen_w[activeBuff + 12] - 48;
        sp->v0 = 160;
        sp->u0 = 0;
        sp->w = 192; sp->h = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 48], sp);

    if (*p != 0) {
        goto_tpage(320, 256, 12, 0, 2);
    } else {
        goto_tpage(704, 0, 12, 0, 2);
    }

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    SetSemiTrans(sp, 0);
    SetShadeTex(sp, 0);

    sp->r0 = bg_change_tbl2;
    sp->g0 = bg_change_tbl2;
    sp->b0 = bg_change_tbl2;

    if (p == &bg_change_tbl1) {
        sp->x0 = screen_w[activeBuff + 10] + 32;
        sp->y0 = screen_w[activeBuff + 12] - 120;
        sp->v0 = 0;
        sp->u0 = 0;
        sp->w = 128; sp->h = 160;
    } else {
        sp->x0 = screen_w[activeBuff + 10] + 32;
        sp->y0 = screen_w[activeBuff + 12] - 48;
        sp->v0 = 160;
        sp->u0 = 0;
        sp->w = 128; sp->h = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 48], sp);

    if (*p != 0) {
        goto_tpage(512, 256, 12, 0, 2);
    } else {
        goto_tpage(896, 0, 12, 0, 2);
    }

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    SetSemiTrans(sp, 0);
    SetShadeTex(sp, 0);

    sp->r0 = bg_change_tbl2;
    sp->g0 = bg_change_tbl2;
    sp->b0 = bg_change_tbl2;

    if (p == &bg_change_tbl1) {
        sp->x0 = screen_w[activeBuff + 10] - 160;
        sp->y0 = screen_w[activeBuff + 12] - 120;
        sp->v0 = 0;
        sp->u0 = 0;
        sp->w = 192; sp->h = 160;
    } else {
        sp->x0 = screen_w[activeBuff + 10] - 160;
        sp->y0 = screen_w[activeBuff + 12] - 48;
        sp->v0 = 160;
        sp->u0 = 0;
        sp->w = 192; sp->h = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 52], sp);

    if (*p == 0) {
        goto_tpage(320, 256, 13, 0, 2);
    } else {
        goto_tpage(704, 0, 13, 0, 2);
    }

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    SetSemiTrans(sp, 0);
    SetShadeTex(sp, 0);

    sp->r0 = bg_change_tbl2;
    sp->g0 = bg_change_tbl2;
    sp->b0 = bg_change_tbl2;

    if (p == &bg_change_tbl1) {
        sp->x0 = screen_w[activeBuff + 10] + 32;
        sp->y0 = screen_w[activeBuff + 12] - 120;
        sp->v0 = 0;
        sp->u0 = 0;
        sp->w = 128; sp->h = 160;
    } else {
        sp->x0 = screen_w[activeBuff + 10] + 32;
        sp->y0 = screen_w[activeBuff + 12] - 48;
        sp->v0 = 160;
        sp->u0 = 0;
        sp->w = 128; sp->h = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 52], sp);

    if (*p == 0) {
        goto_tpage(512, 256, 13, 0, 2);
        return;
    }
    goto_tpage(896, 0, 13, 0, 2);
}
