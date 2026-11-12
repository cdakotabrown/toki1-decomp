/* @SymbolStart: 0x80133478 */
/* @SymbolSize: 80 */
/* func_80133480 -- GYOZI overlay, Address: 0x80133478, Size: 80 bytes (20 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_80145EC4;
extern u8 D_800F647A;

void func_80133480(void)
{
    u32 temp = D_80145EC4;

    if (temp != 0) goto label0;
    func_8008A0D4(0x4107);
    goto label1;

label0:
    D_800F647A = (D_800F647A + 3);

label1:
    func_8004DE1C();
}
