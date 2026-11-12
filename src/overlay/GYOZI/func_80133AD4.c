/* @SymbolStart: 0x80133ACC */
/* @SymbolSize: 100 */
/* func_80133AD4 -- GYOZI overlay, Address: 0x80133ACC, Size: 100 bytes (25 insns) */
#include "types.h"

extern void func_8004DDD8(void);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern u8 D_80145EC8;
extern u8 D_80145F60;

void func_80133AD4(void)
{
    u32 temp = D_800F62CF;

    if (temp != 5) goto label0;
    if (D_80145F60 != 0) goto label0;
    if (D_80145EC8 != 0) goto label1;

label0:
    func_8004DDD8();
    goto label2;

label1:
    func_8004DE1C();

label2:
    ;
}
