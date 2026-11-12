/* @SymbolStart: 0x80133B30 */
/* @SymbolSize: 64 */
/* func_80133B38 -- GYOZI overlay, Address: 0x80133B30, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80145F60;
extern u8 D_800F647A;

void func_80133B38(void)
{
    u32 temp = D_80145F60;

    if (temp == 0) goto label0;
    D_800F647A = D_800F647A + 4;

label0:
    func_8004DE1C();
}
