/* @SymbolStart: 0x80132D1C */
/* @SymbolSize: 128 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:v0 0x28:rs:v1:v0 0x28:rt:v1:v0 0x30:rs:v1:v0 0x38:rs:v1:v0 0x40:rs:v1:v0 0x48:rs:v1:v0 0x50:rs:v1:v0 */
/* func_80132D24 -- GYOZI overlay, Address: 0x80132D1C, Size: 128 bytes (32 insns) */
#include "types.h"

extern void func_8008E310(void);
extern void func_8008E2E0(void);
extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern u8 D_800F62CF;

void func_80132D24(void)
{
    s32 temp = D_8012E66C;
    u8 val;

    if (temp != 0) {
        val = D_800F62CF;
        if (val == 2) goto do_e310;
        if (val == 7) goto do_e310;
        if (val == 8) goto do_e310;
        if (val == 9) goto do_e310;
        if (val != 10) goto do_e2e0;
    do_e310:
        func_8008E310();
        return;
    do_e2e0:
        func_8008E2E0();
        return;
    }

    func_8004DE1C();
}
