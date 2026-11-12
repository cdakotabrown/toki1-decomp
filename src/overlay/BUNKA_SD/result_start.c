/* result_start - BUNKA_SD overlay, Address: 0x80133924, Size: 96 bytes (24 insns) */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t6:at */
/* @PostProc: patch_insn_reg_field 0x2C:rs:t6:at */
/* @PostProc: patch_insn_reg_field 0x30:rt:t8:at */
/* @PostProc: patch_insn_reg_field 0x34:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x38:rs:t8:at 0x38:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x3C:rt:t9:at */
/* @PostProc: patch_insn_reg_field 0x40:rs:t9:at */
/* @PostProc: patch_insn_reg_field 0x44:rt:t0:at */
/* @PostProc: patch_insn_reg_field 0x48:rs:t0:at */
/* @PostProc: patch_insn_reg_field 0x4C:rt:t1:at */
/* @PostProc: patch_insn_reg_field 0x50:rs:t1:at */
/* @PostProc: patch_insn_reg_field 0x54:rt:t2:at */
/* @PostProc: patch_insn_reg_field 0x5C:rs:t2:at */
#include "types.h"

extern void result_init(void);
extern void k_speed_set(s32);
extern void sub_sub_step_up(void);

void result_start(void)
{
    result_init();
    k_speed_set(5);
    *(s32 *)0x8013C638U = 0;
    *(s32 *)0x8013C644U = 1;
    *(s32 *)0x8013C648U = 0;
    *(s32 *)0x8013C64CU = 0;
    *(s32 *)0x8013C650U = 0;
    *(s32 *)0x8013C654U = 0;
    sub_sub_step_up();
}
