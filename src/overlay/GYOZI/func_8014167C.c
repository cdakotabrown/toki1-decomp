/* @SymbolStart: 0x80141674 */
/* @SymbolSize: 72 */
/* func_8014167C -- GYOZI overlay, Address: 0x80141674, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F563A;
extern s16 D_80148620;

void func_8014167C(void)
{
    u32 temp = D_800F563A;

    if ((temp >> 4) != 7) goto label0;
    D_80148620 = (D_80148620 + 3);

label0:
    func_8004DE1C();
}
