/* @SymbolSize: 128 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 0x2c:rs:v1:t6 0x30:rt:t6:t7 0x38:rs:v1:t6 0x3c:rt:t8:t9 0x44:rs:v1:t6 0x54:rs:t6:t7 0x54:rt:t6:t7 0x5c:rs:t6:t7 0x5c:rt:t7:t8 0x64:rt:t7:t8 0x68:rs:t8:t9 0x68:rt:t8:t9 0x70:rs:t8:t9 0x70:rt:t9:t0 0x74:rt:t9:t0 */
/* func_8013405C -- GYOZI overlay, Address: 0x8013405C, Size: 128 bytes (32 insns) */
#include "types.h"

extern s32 system_config_disp(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s16 D_80145EB4;

void func_8013405C(void)
{
    s32 sel;

    sel = system_config_disp(D_800F62CF) & 0x7F;

    switch (sel) {
    case 0:
        D_80145EB4 = D_80145EB4 + 2;
        break;
    case 1:
    case 2:
        D_80145EB4 = D_80145EB4 + 1;
        break;
    }

    func_8004DE1C();
}
