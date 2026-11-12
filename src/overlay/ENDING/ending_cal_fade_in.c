/* @SymbolStart: 0x80134B28 */
/* @SymbolSize: 216 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:t8,3:t9,4:t7 IDO52_FORCE_WEB_COLOR=23:a1 IDO52_AS1_NO_SCHED_LOAD_HOIST=strict:3 IDO52_AS1_TIE_TAKE=25,47 */
/* @PostProc: patch_insn_reg_field 0x30:rd:v0:v1 0x40:rt:v0:v1 0x48:rt:v0:v1 0x5c:rt:v0:v1 0x74:rt:v0:v1 */
/* ending_cal_fade_in - ENDING overlay, Address: 0x80134B28, Size: 216 bytes (54 instructions) */
#include "types.h"

extern void sprite_rgb_set();
extern void sub_sub_step_up();
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 cal_cw_mode;
extern u8 cw97_bright;
extern volatile u8 cw97_bright_v;
extern volatile u8 cal_cw_bright_v;

void ending_cal_fade_in(void)
{
    s32 v1;
    s32 s0;
    u8  t8;
    u8  a1;

    v1 = (back_clear_r + 8) & 0xFF;
    if (v1 == 0 || v1 == 7) {
        v1 = 255;
    }
    back_clear_b = (u8)v1;
    t8 = cw97_bright;
    back_clear_g = (u8)v1;
    a1 = cal_cw_bright_v;
    cw97_bright_v = (u8)(t8 + 4);
    back_clear_r = (u8)v1;
    cal_cw_bright_v = (u8)(a1 + 4);
    s0 = 4;
    do {
        sprite_rgb_set((s32)(s16)s0, cal_cw_bright_v);
        s0 = (s16)(s0 + 1);
    } while (s0 < 10);
    a1 = cal_cw_bright_v;
    if ((u32)a1 >= 128) {
        cal_cw_mode = 5;
        sub_sub_step_up();
    }
}
