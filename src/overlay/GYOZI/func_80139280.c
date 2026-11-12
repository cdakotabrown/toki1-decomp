/* @SymbolStart: 0x80139278 */
/* @SymbolSize: 64 */
/* func_80139280 -- GYOZI overlay, Address: 0x80139278, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80147650;
extern s16 D_801474A8;

void func_80139280(void)
{
    u32 temp = D_80147650;

    if (temp == 0) goto label0;
    D_801474A8 = D_801474A8 + 1;

label0:
    func_8004DE1C();
}
