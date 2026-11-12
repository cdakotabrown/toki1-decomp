/* @SymbolStart: 0x8013ADD0 */
/* @SymbolSize: 60 */
/* func_8013ADD8 -- GYOZI overlay, Address: 0x8013ADD0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8013CE0C(void);
extern void func_8013CEB0(void);

extern u8 D_801474B8;

void func_8013ADD8(void)
{
    u32 temp = D_801474B8;

    if (temp != 0) goto label0;
    func_8013CE0C();
    goto label1;

label0:
    func_8013CEB0();

label1:
    ;
}
