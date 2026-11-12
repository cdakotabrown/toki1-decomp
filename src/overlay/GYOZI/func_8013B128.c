/* @SymbolStart: 0x8013B120 */
/* @SymbolSize: 60 */
/* func_8013B128 -- GYOZI overlay, Address: 0x8013B120, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8013D15C(void);
extern void func_8013D20C(void);

extern u8 D_801474B8;

void func_8013B128(void)
{
    u32 temp = D_801474B8;

    if (temp != 0) goto label0;
    func_8013D15C();
    goto label1;

label0:
    func_8013D20C();

label1:
    ;
}
