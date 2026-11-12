/* @SymbolStart: 0x8013B9A8 */
/* @SymbolSize: 64 */
/* func_8013B9B0 -- GYOZI overlay, Address: 0x8013B9A8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F563A;
extern s16 D_801474A8;

void func_8013B9B0(void)
{
    u32 temp = D_800F563A;

    if ((temp >> 4) != 7) goto label0;
    D_801474A8 = 6;

label0:
    func_8004DE1C();
}
