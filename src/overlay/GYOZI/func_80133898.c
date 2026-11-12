/* @SymbolStart: 0x80133890 */
/* @SymbolSize: 60 */
/* func_80133898 -- GYOZI overlay, Address: 0x80133890, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8004DDD8(void);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;

void func_80133898(void)
{
    u32 temp = D_800F62CF;

    if (temp == 0) goto label0;
    func_8004DDD8();
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
