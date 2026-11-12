/* @SymbolStart: 0x8013EDCC */
/* @SymbolSize: 64 */
/* func_8013EDD4 -- GYOZI overlay, Address: 0x8013EDCC, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80148750;
extern s16 D_80148620;

void func_8013EDD4(void)
{
    u32 temp = D_80148750;

    if (temp == 0) goto label0;
    D_80148620 = D_80148620 + 1;

label0:
    func_8004DE1C();
}
