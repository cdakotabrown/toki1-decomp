/* @SymbolStart: 0x8013C570 */
/* @SymbolSize: 64 */
/* func_8013C578 -- GYOZI overlay, Address: 0x8013C570, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DDD8(void);
extern void func_8004DE1C(void);

extern u8 D_80148110;

void func_8013C578(void)
{
    u32 temp = D_80148110;

    if (temp == 0) goto label0;
    func_8004DDD8();
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
