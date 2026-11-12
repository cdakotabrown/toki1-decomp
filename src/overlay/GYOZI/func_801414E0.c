/* @SymbolStart: 0x801414D8 */
/* @SymbolSize: 72 */
/* func_801414E0 -- GYOZI overlay, Address: 0x801414D8, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F55CA;
extern s16 D_80148620;

void func_801414E0(void)
{
    u32 temp = D_800F55CA;

    if ((temp >> 4) == 3) goto label0;
    D_80148620 = (D_80148620 + 1);

label0:
    func_8004DE1C();
}
