/* @SymbolStart: 0x8013C69C */
/* @SymbolSize: 208 */
/* Function: date_basyo12_face_para_step */
/* Overlay: DATE */
/* Address: 0x8013C69C (cross-prologue head) / 0x8013C6A4 (body) */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

extern u8 now_girl;
extern volatile s16 girl2_tokimeki;
extern volatile s16 girl2_yuukou;
extern u8 girl1_spot26_cnt;

extern s32 bustup_face_step0();
extern s32 bustup_face_step1();
extern s32 bustup_face_step3();
extern s32 param_clamp();

s32 date_basyo12_face_para_step(void)
{
    s32 v0;

    v0 = (s32)now_girl;
    if (v0 == 2) {
        bustup_face_step3();
        girl2_tokimeki = girl2_tokimeki - 1;
        girl2_yuukou = girl2_yuukou - 1;
    } else if (v0 == 1 && girl1_spot26_cnt == 0) {
        bustup_face_step0();
        girl2_tokimeki = girl2_tokimeki + 1;
        girl2_yuukou = girl2_yuukou + 1;
    } else {
        bustup_face_step1();
        girl2_tokimeki = girl2_tokimeki + 1;
    }
    return param_clamp();
}
