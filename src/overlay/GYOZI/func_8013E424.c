/* @SymbolStart: 0x8013E41C */
/* @SymbolSize: 72 */
/* func_8013E424 -- GYOZI overlay, Address: 0x8013E41C, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8013FD20(void);
extern void func_8004DE1C(void);

extern u8 D_800F594F;

void func_8013E424(void)
{
    u32 temp = D_800F594F;

    if (temp != 1) goto label0;
    func_8013FD20();
    goto label1;

label0:
    func_8004DE1C();
    func_8004DE1C();

label1:
    ;
}
