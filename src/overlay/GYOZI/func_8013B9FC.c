/* @SymbolStart: 0x8013B9E8 */
/* @SymbolSize: 112 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:t8 0x14:rs:t6:t8 0x14:rt:t6:t8 0x18:rt:t9:t6 0x1c:rs:t9:t6 0x1c:rt:t9:t6 0x20:rd:t7:t9 0x20:rt:t6:t8 0x28:rd:t8:t0 0x28:rt:t7:t9 0x2c:rd:t0:t7 0x2c:rt:t9:t6 0x30:rs:t8:t7 */
/* Address: 0x8013B9E8 (prologue 0x8013B9FC), Size: 112 bytes (28 insns) */
#include "types.h"

extern u32 D_800F5638;
extern u32 D_800F5488;
extern u8 D_800F563A;
extern s16 D_801474A8;

extern void func_8004DE1C(void);

void func_8013B9FC(void)
{
    u32 t1;

    if ((D_800F5488 >> 28) != ((D_800F5638 << 29) >> 31)) goto end;
    t1 = (u8)D_800F563A;
    if ((t1 >> 4) != 7) goto store_8;
    D_801474A8 = 10;
    goto end;
store_8:
    D_801474A8 = 8;
end:
    func_8004DE1C();
}
