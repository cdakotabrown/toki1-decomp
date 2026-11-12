/* @SymbolStart: 0x801334F4 */
/* @SymbolSize: 92 */
/* @PostProc: patch_insn_reg_field 0x1c:rd:t7:t8 0x20:rs:t7:t8 */
/* ending_girl1_serifu_step - ENDING overlay, Address: 0x801334F4, Size: 92 bytes (23 instructions) */
#include "types.h"

extern s32 girl1_flag;
extern s32 get_h_tokimeki();
extern void serifu_run();
extern void sub_sub_step_up();

void ending_girl1_serifu_step(void)
{
    s32 flags = girl1_flag;
    s32 v0;

    if ((flags << 30) >= 0) {
        goto advance;
    }
    v0 = get_h_tokimeki(1);
    if ((u32)v0 < 70) {
        goto advance;
    }
    serifu_run();
    return;
advance:
    sub_sub_step_up();
}
