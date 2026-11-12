/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4C */
/* @PostProc: patch_insn_reg_field 0x30:rt:t7:t8 0x34:rs:t7:t8 0x40:rt:v0:v1 0x48:rt:t8:v0 0x48:rs:v0:v1 0x4C:rs:v0:v1 0x50:rs:t8:v0 */
/* func_80135854 -- GYOZI overlay, Address: 0x80135854, Size: 124 bytes (31 insns) */
#include "types.h"

extern s32 system_config_disp(s32);
extern void func_801343A4(void);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s32 D_800F6474;
extern s16 D_80145EB4;

void func_80135854(void)
{
    s32 ret;
    s32 v1;
    s32 cond;

    ret = system_config_disp(5);
    if (D_800F62CF != 5) goto L_else;
    if ((u32)(ret & 0x7F) >= 2U) goto L_else;
    v1 = D_800F6474;
    D_800F6474 = v1 + 1;
    cond = ((u32)v1 < 1U);
    if (cond == 0) goto L_skip;
    D_80145EB4 = 42;
L_skip:
    func_801343A4();
    return;
L_else:
    func_8004DE1C();
}
