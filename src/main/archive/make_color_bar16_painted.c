/* Function: bg_show_girl_out (obsolete painted panel code, not an RGB555 gradient) */
/* Address: 0x800580A4 */
/* Size: 1648 bytes (412 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 56 --new 80 */
/* @PostProc: remap_stack_slots --map 44:60 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_make_color_bar16.py */
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
    u8 pad0[4];
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 pad7;
    s16 field_8;
    s16 field_A;
    u8 field_C;
    u8 field_D;
    u8 pad_E[2];
    s16 field_10;
    s16 field_12;
} SpriteStruct20;

void bg_show_girl_out(s32 arg0) {
    SpriteStruct20 *s0;
    u8 *sp3C;

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 1);
    SetShadeTex(s0, 0);

    s0->field_4 = 128 - bg_change_tbl2;
    s0->field_5 = 128 - bg_change_tbl2;
    s0->field_6 = 128 - bg_change_tbl2;

    if (arg0 == 1) {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_10 = 192;
        s0->field_12 = 160;
    } else {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 48;
        s0->field_D = 160;
        s0->field_C = 0;
        s0->field_10 = 192;
        s0->field_12 = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 40], s0);

    sp3C = &bg_change_tbl0 + arg0;

    if (*sp3C != 0) {
        goto_tpage(320, 256, 10, 1, 2);
    } else {
        goto_tpage(704, 0, 10, 1, 2);
    }

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 1);
    SetShadeTex(s0, 0);

    s0->field_4 = 128 - bg_change_tbl2;
    s0->field_5 = 128 - bg_change_tbl2;
    s0->field_6 = 128 - bg_change_tbl2;

    if (sp3C == &bg_change_tbl1) {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_12 = 160;
        s0->field_10 = 128;
    } else {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 48;
        s0->field_D = 160;
        s0->field_C = 0;
        s0->field_10 = 128;
        s0->field_12 = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 40], s0);

    if (*sp3C != 0) {
        goto_tpage(512, 256, 10, 1, 2);
    } else {
        goto_tpage(896, 0, 10, 1, 2);
    }

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 0);
    SetShadeTex(s0, 0);

    s0->field_4 = bg_change_tbl2;
    s0->field_5 = bg_change_tbl2;
    s0->field_6 = bg_change_tbl2;

    if (sp3C == &bg_change_tbl1) {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_12 = 160;
        s0->field_10 = 192;
    } else {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 48;
        s0->field_D = 160;
        s0->field_C = 0;
        s0->field_10 = 192;
        s0->field_12 = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 52], s0);

    if (*sp3C != 0) {
        goto_tpage(320, 256, 13, 0, 2);
    } else {
        goto_tpage(704, 0, 13, 0, 2);
    }

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 0);
    SetShadeTex(s0, 0);

    s0->field_4 = bg_change_tbl2;
    s0->field_5 = bg_change_tbl2;
    s0->field_6 = bg_change_tbl2;

    if (sp3C == &bg_change_tbl1) {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_12 = 160;
        s0->field_10 = 128;
    } else {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 48;
        s0->field_D = 160;
        s0->field_C = 0;
        s0->field_10 = 128;
        s0->field_12 = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 52], s0);

    if (*sp3C != 0) {
        goto_tpage(512, 256, 13, 0, 2);
        return;
    }
    goto_tpage(896, 0, 13, 0, 2);
}
