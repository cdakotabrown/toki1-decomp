/* Address: 0x8014BC50 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: patch_insn_reg_field 0x34:rd:s0:s2 0x38:rd:s2:s0 0x44:rs:s0:s2 0x50:rt:s2:s0 0x50:rs:s2:s0 0x58:rs:s2:s0 0x5c:rt:s0:s2 0x5c:rs:s0:s2 */
#include "types.h"

extern s32 taco_step_ang_tbl[];

extern void angle_set();

void taco_angles_set(s32 a0, s32 a1)
{
    s32 *ptr;
    s32 i;
    s32 idx;

    ptr = &taco_step_ang_tbl[a1];
    idx = a0;
    for (i = 0; i < 3; i++) {
        angle_set(idx, *ptr);
        idx++;
        ptr++;
    }
}
