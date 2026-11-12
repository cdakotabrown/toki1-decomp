/* bunka_sd_rec_start - BUNKA_SD overlay, Address: 0x80136EDC, Size: 96 bytes (24 insns) */
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

extern void bunka_sd_rec_init(void);
extern void k_speed_set(s32);
extern void sub_sub_step_up(void);

void bunka_sd_rec_start(void)
{
    bunka_sd_rec_init();
    k_speed_set(5);
    *(u32*)0x8013CE2C = 0;
    *(u32*)0x8013CE38 = 1;
    *(u32*)0x8013CE3C = 0;
    *(u32*)0x8013CE40 = 0;
    *(u32*)0x8013CE44 = 0;
    *(u32*)0x8013CE48 = 0;
    sub_sub_step_up();
}
