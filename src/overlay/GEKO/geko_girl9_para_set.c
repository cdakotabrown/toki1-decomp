/* geko_girl9_para_set -- GEKO overlay, Address: 0x8013E0C0, Size: 140 bytes (35 insns) */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x58:rt:t0:t1 0x60:rs:t0:t1 0x60:rt:t1:t2 0x64:rt:t1:t2 0x68:rt:t2:t3 0x6c:rs:t2:t3 0x6c:rt:t2:t3 0x74:rs:t2:t3 0x74:rt:t3:t4 0x78:rt:t3:t4 */
#include "types.h"

extern s32 get_h_yuukou(s32 a0);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 girl9_tokimeki;
extern s16 girl9_yuukou;
extern u8 girl9_flag_hi;

void geko_girl9_para_set(void)
{
    s32 v0;

    v0 = get_h_yuukou(9);
    if ((u32)v0 >= 80) {
        girl9_tokimeki += 1;
        girl9_yuukou += 2;
    } else {
        girl9_flag_hi |= 0x40;
        girl9_yuukou += 1;
    }
    param_clamp();
    sub_sub_step_up();
}
