/* func_80135A0C -- GYOZI overlay, Address: 0x80135A0C, Size: 116 bytes (29 insns) */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 0x2c:rs:t6:t7 0x38:rt:t7:t8 0x3c:rs:t7:t8 0x3c:rt:t7:t8 0x44:rs:t7:t8 0x44:rt:t8:t9 0x48:rt:t8:t9 0x4c:rs:t9:t0 0x4c:rt:t9:t0 0x54:rs:t9:t0 0x58:rt:t9:t1 0x5c:rs:t0:t1 0x5c:rt:t0:t1 0x64:rs:t0:t1 0x64:rt:t1:t2 0x68:rt:t1:t2 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x58 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t1:t0 0x58:rt:t0:t1 */
#include "types.h"

extern s32 system_config_disp(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_800F53DE;
extern u8 D_800F62CF;
extern s16 D_80145EB4;

void func_80135A0C(void)
{
    s32 code;

    code = system_config_disp(D_800F62CF) & 0x7F;

    if ((u32)code >= 2) {
        D_80145EB4 = D_80145EB4 + 2;
    }

    if (D_800F53DE == 98) {
        D_80145EB4 = D_80145EB4 + 1;
    }

    func_8004DE1C();
}
