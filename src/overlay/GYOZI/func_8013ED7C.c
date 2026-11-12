/* @SymbolSize: 80 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 0x30:rs:t6:t7 */
/* func_8013ED7C -- GYOZI overlay, Address: 0x8013ED7C, Size: 80 bytes (20 insns) */
#include "types.h"

extern s32 system_config_disp(u8);
extern void func_8004DE1C(void);
extern void func_8013FD20(void);

extern u8 D_800F62CF;

void func_8013ED7C(void)
{
    s32 v;

    v = system_config_disp(D_800F62CF);
    if ((v & 0x7F) == 4) {
        goto label;
    }
    func_8004DE1C();
    return;
label:
    func_8013FD20();
}
