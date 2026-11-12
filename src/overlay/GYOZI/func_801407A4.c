/* @SymbolStart: 0x80140790 */
/* @SymbolSize: 84 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:t7 0x1c:rd:v0:t7 0x1c:rs:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x2c:rs:v0:t7 */
/* func_801407A4 -- GYOZI overlay, Address: 0x801407A4, Size: 64 bytes (16 insns) */
#include "types.h"

extern u8 D_80148818;
extern u8 D_80148828;
extern void func_80086AB0(s32 a0);
extern void func_8004DE1C(void);

void func_801407A4(void)
{
    u8 t7 = (&D_80148828)[D_80148818];

    if (t7 != 11) goto label0;
    func_80086AB0(0x200);
    goto label1;

label0:
    func_80086AB0(0x201);

label1:
    func_8004DE1C();
}
