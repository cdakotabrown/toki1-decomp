/* @SymbolStart: 0x8013C4F4 */
/* @SymbolSize: 64 */
/* func_8013C4FC -- GYOZI overlay, Address: 0x8013C4F4, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80148110;
extern s16 D_80148108;

void func_8013C4FC(void)
{
    u32 temp = D_80148110;

    if (temp != 0) goto label0;
    D_80148108 = D_80148108 + 1;

label0:
    func_8004DE1C();
}
