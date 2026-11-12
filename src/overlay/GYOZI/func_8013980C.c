/* @SymbolStart: 0x80139804 */
/* @SymbolSize: 68 */
/* func_8013980C -- GYOZI overlay, Address: 0x80139804, Size: 68 bytes (17 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_80147650;

void func_8013980C(void)
{
    u32 temp = D_80147650;

    if (temp == 0) goto label0;
    func_8008A0D4(0x406F);
    goto label1;

label0:
    func_8008A0D4(0x4065);

label1:
    func_8004DE1C();
}
