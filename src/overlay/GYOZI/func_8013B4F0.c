/* @SymbolStart: 0x8013B4E8 */
/* @SymbolSize: 64 */
/* func_8013B4F0 -- GYOZI overlay, Address: 0x8013B4E8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_800F5488;
extern s16 D_801474A8;

void func_8013B4F0(void)
{
    s32 temp = D_800F5488;

    if (((u32)temp >> 28) != 9) goto label0;
    D_801474A8 = 5;

label0:
    func_8004DE1C();
}
