/* @SymbolStart: 0x80133BAC */
/* @SymbolSize: 84 */
/* func_80133BB4 -- GYOZI overlay, Address: 0x80133BAC, Size: 84 bytes (21 insns) */
#include "types.h"

extern void func_801343A4(void);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern u8 D_80145EC8;

void func_80133BB4(void)
{
    u32 temp = D_800F62CF;

    if (temp != 5) goto label0;
    if (D_80145EC8 != 0) goto label0;
    func_801343A4();
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
