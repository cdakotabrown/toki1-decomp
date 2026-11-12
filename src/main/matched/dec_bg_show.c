/* Function: dec_bg_show */
/* Likely Symbol: dec_bg_show */
/* Address: 0x80057C5C */
/* Size: 1084 bytes (271 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void printf(void *);
extern void *prim_alloc(s32 size, s32 a1);
extern void SetSprt(void *a0);
extern void SetSemiTrans(void *a0, s32 a1);
extern void SetShadeTex(void *a0, s32 a1);
extern void AddPrim(void *a0, void *a1);
extern void goto_tpage(s32, s32, s32, s32, s32);

extern s32 cd_wait_timeout_tbl0;
extern u8 bg_change_tbl0;
extern u8 bg_change_tbl1;
extern u8 dec_bg_show_tbl0;
extern u8 fade_bri;
extern s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];
extern u8 dec_bg_show_str0[];

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

void dec_bg_show(s32 arg0) {
    s32 unused;
    SpriteStruct20 *s0;
    u8 *sp38;

    if (arg0 == -1) {
        printf(dec_bg_show_str0);
        return;
    }

    cd_wait_timeout_tbl0 = arg0;

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 0);
    SetShadeTex(s0, 0);

    s0->field_4 = fade_bri;
    s0->field_5 = fade_bri;
    s0->field_6 = fade_bri;

    if (arg0 == 1) {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_10 = 192;
        s0->field_12 = 160;
    } else if (arg0 == 2) {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_10 = 192;
        s0->field_12 = 240;
    } else {
        s0->field_8 = screen_w[activeBuff + 10] - 160;
        s0->field_A = screen_w[activeBuff + 12] - 48;
        s0->field_D = 160;
        s0->field_C = 0;
        s0->field_10 = 192;
        s0->field_12 = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 52], s0);

    sp38 = &bg_change_tbl0 + arg0;

    if (*sp38 != 0) {
        goto_tpage(320, 256, 13, 0, 2);
    } else {
        goto_tpage(704, 0, 13, 0, 2);
    }

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 0);
    SetShadeTex(s0, 0);

    s0->field_4 = fade_bri;
    s0->field_5 = fade_bri;
    s0->field_6 = fade_bri;

    if (sp38 == &bg_change_tbl1) {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_10 = 128;
        s0->field_12 = 160;
    } else if (sp38 == &dec_bg_show_tbl0) {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 120;
        s0->field_D = 0;
        s0->field_C = 0;
        s0->field_10 = 128;
        s0->field_12 = 240;
    } else {
        s0->field_8 = screen_w[activeBuff + 10] + 32;
        s0->field_A = screen_w[activeBuff + 12] - 48;
        s0->field_D = 160;
        s0->field_C = 0;
        s0->field_10 = 128;
        s0->field_12 = 88;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + 52], s0);

    if (*sp38 != 0) {
        goto_tpage(512, 256, 13, 0, 2);
        return;
    }
    goto_tpage(896, 0, 13, 0, 2);
}
