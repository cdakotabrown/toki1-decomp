/* func_80133FE8 -- GYOZI overlay, Address: 0x80133FE8, Size: 116 bytes (29 insns) */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 0x2c:rs:v1:t6 0x34:rs:v1:t6 0x38:rt:t8:t9 0x40:rs:v1:t6 0x44:rt:t6:t7 0x48:rs:t6:t7 0x48:rt:t6:t7 0x50:rs:t6:t7 0x50:rt:t7:t8 0x58:rt:t7:t8 0x5c:rs:t8:t9 0x5c:rt:t8:t9 0x64:rs:t8:t9 0x64:rt:t9:t0 0x68:rt:t9:t0 */
#include "types.h"

extern s32 system_config_disp(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s16 D_80145EB4;

void func_80133FE8(void)
{
    s32 code;

    code = system_config_disp(D_800F62CF) & 0x7F;

    if (code != 0) {
        if (code != 1 && code != 2) {
            D_80145EB4 = D_80145EB4 + 2;
        } else {
            D_80145EB4 = D_80145EB4 + 1;
        }
    }

    func_8004DE1C();
}
