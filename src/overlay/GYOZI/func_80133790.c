/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 0x2c:rs:v1:t6 0x34:rs:v1:t6 0x38:rt:t6:t7 0x40:rs:v1:t6 0x48:rs:v1:t6 0x4c:rt:t8:t9 0x58:rs:t6:t7 0x58:rt:t6:t7 0x60:rs:t6:t7 0x60:rt:t7:t8 0x68:rt:t7:t8 0x6c:rs:t8:t9 0x6c:rt:t8:t9 0x74:rs:t8:t9 0x74:rt:t9:t0 0x78:rt:t9:t0 */
/* func_80133790 -- GYOZI overlay, Address: 0x80133790, Size: 132 bytes (33 insns) */
#include "types.h"

extern s32 system_config_disp(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s16 D_80145EB4;

void func_80133790(void)
{
    s32 sel;

    sel = system_config_disp(D_800F62CF) & 0x7F;

    switch (sel) {
    case 0:
        break;
    case 1:
    case 2:
        D_80145EB4 = D_80145EB4 + 1;
        break;
    case 3:
        D_80145EB4 = D_80145EB4 + 2;
        break;
    }

    func_8004DE1C();
}
