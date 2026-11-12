/* @SymbolStart: 0x80134C44 */
/* @SymbolSize: 248 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x20:rs:v0:v1 0x50:rt:t7:t8 0x54:rs:t7:t8 0x54:rt:t7:t8 0x5c:rt:v0:a1 0x64:rs:v0:a1 0x64:rt:v0:a1 0x6c:rs:t7:t8 0x6c:rt:t8:t9 0x80:rs:v0:a1 0x80:rt:t9:t7 0x84:rt:t8:t7 0x8c:rt:v0:a1 0x98:rs:v0:a1 0xbc:rt:v0:a1 0xc0:rs:v0:a1 0xc0:rt:v0:a1 0xc8:rs:v0:a1 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x84 */
/* @PostProc: patch_insn_reg_field 0x70:rt:t7:t9 0x84:rt:t9:t7 */
/* ending_cal_fade_out - ENDING overlay, Address: 0x80134C44, Size: 248 bytes (62 instructions) */
#include "types.h"

extern void sprite_rgb_set(s32, s32);
extern void sub_sub_step_up(void);
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern u8 cw97_disp;
extern u8 cw97_bright;

void ending_cal_fade_out(void)
{
    s32 v1 = back_clear_r;
    s32 t6;
    s32 b7b;
    s32 b37;
    s16 i;

    v1 -= 8;
    t6 = v1 & 0xFF;
    v1 = t6;
    if (t6 == 7 || t6 == 248) {
        v1 = 0;
    }
    back_clear_b = (u8)v1;
    b7b = cw97_bright;
    back_clear_g = (u8)v1;
    b37 = cal_cw_bright;
    cw97_bright = (u8)(b7b - 4);
    back_clear_r = (u8)v1;
    cal_cw_bright = (u8)(b37 - 4);
    for (i = 4; i < 10; i++) {
        sprite_rgb_set((s16)(i + 0), cal_cw_bright);
    }
    if ((u32)cal_cw_bright < 4U) {
        cal_cw_disp = (u8)(cal_cw_disp & 0xFF7F);
        cw97_disp = (u8)(cw97_disp & 0xFF7F);
        sub_sub_step_up();
    }
}
