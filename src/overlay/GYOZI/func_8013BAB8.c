/* @SymbolStart: 0x8013BAA4 */
/* @SymbolSize: 168 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:t8 0x14:rs:t6:t8 0x14:rt:t6:t8 0x18:rt:t9:t6 0x1c:rs:t9:t6 0x1c:rt:t9:t6 0x20:rd:t7:t9 0x20:rt:t6:t8 0x28:rd:t8:t0 0x28:rt:t7:t9 0x2c:rd:t0:t7 0x2c:rt:t9:t6 0x30:rs:t8:t7 */
/* Address: 0x8013BAA4 (prologue 0x8013BAB8), Size: 168 bytes (42 insns) + a */
#include "types.h"

extern u32 D_800F5638;
extern u32 D_800F5488;

extern void func_8004DE1C(void);

void func_8013BAB8(void)
{
    if ((D_800F5488 >> 28) != ((D_800F5638 << 29) >> 31)) goto label0;
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
