/* @SymbolStart: 0x8013BCDC */
/* @SymbolSize: 72 */
/* geko_wait17_step -- GEKO overlay, Address: 0x8013BCE4, Size: 64 bytes (16 insns) */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:t6:v0 0x20:rs:t6:v0 0x20:rt:t7:v0 0x28:rs:v0:v1 0x30:rs:t7:v0 */
#include "types.h"

extern void k_reset(s32 a0);
extern void sub_sub_step_up(void);

extern s32 step_counter;

void geko_wait17_step(void)
{
    s32 v1 = step_counter;

    step_counter = v1 + 1;
    if (((u32)v1 < 17) ^ 1) {
        k_reset(1);
        sub_sub_step_up();
    }
}
