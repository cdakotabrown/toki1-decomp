/* @SymbolStart: 0x80133C00 */
/* @SymbolSize: 60 */
/* func_80133C08 -- GYOZI overlay, Address: 0x80133C00, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_801343A4(void);
extern void func_8004DE1C(void);

extern u8 D_80145EC8;

void func_80133C08(void)
{
    u32 temp = D_80145EC8;

    if (temp == 0) goto label0;
    func_801343A4();
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
