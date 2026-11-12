/* @SymbolStart: 0x8013B0D0 */
/* @SymbolSize: 72 */
/* func_8013B0D8 -- GYOZI overlay, Address: 0x8013B0D0, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_80072734(s32);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;

void func_8013B0D8(void)
{
    u32 temp = D_800F62CF;

    if (temp != 5) goto label0;
    func_80072734(24334);
    goto label1;

label0:
    func_80072734(21634);

label1:
    func_8004DE1C();
}
