/* @SymbolStart: 0x80133B70 */
/* @SymbolSize: 60 */
/* func_80133B78 -- GYOZI overlay, Address: 0x80133B70, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8004DE1C(void);
extern void func_801343A4(void);

extern u8 D_80145EC8;

void func_80133B78(void)
{
    u32 temp = D_80145EC8;

    if (temp != 0) goto label0;
    func_8004DE1C();
    goto label1;

label0:
    func_801343A4();

label1:
    ;
}
