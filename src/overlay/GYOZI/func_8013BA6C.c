/* @SymbolStart: 0x8013BA58 */
/* @SymbolSize: 76 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:t8 0x14:rs:t6:t8 0x14:rt:t6:t8 0x18:rt:t9:t6 0x1c:rs:t9:t6 0x1c:rt:t9:t6 0x20:rd:t7:t9 0x20:rt:t6:t8 0x28:rd:t8:t0 0x28:rt:t7:t9 0x2c:rd:t0:t7 0x2c:rt:t9:t6 0x30:rs:t8:t7 */
/* Address: 0x8013BA58 (prologue 0x8013BA6C), Size: 76 bytes (19 insns) */
#include "types.h"

extern u32 D_800F5638;
extern u32 D_800F5488;
extern s16 D_801474A8;

extern void func_8004DE1C(void);

void func_8013BA6C(void)
{
    if ((D_800F5488 >> 28) == ((D_800F5638 << 29) >> 31)) goto label0;
    D_801474A8 = 4;

label0:
    func_8004DE1C();
}
