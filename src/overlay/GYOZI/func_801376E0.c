/* @SymbolStart: 0x801376D8 */
/* @SymbolSize: 52 */
/* func_801376E0 -- GYOZI overlay, Address: 0x801376D8, Size: 52 bytes (13 insns) */
#include "types.h"

extern void func_8004DEE4(s32 a0);

extern s16 D_80147220;

void func_801376E0(void)
{
    s32 temp = D_80147220;

    D_80147220 = (s16)(temp + 1);
    func_8004DEE4(1);
}
