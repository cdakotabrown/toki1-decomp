/* @SymbolStart: 0x80135D3C */
/* @SymbolSize: 52 */
/* func_80135D44 -- GYOZI overlay, Address: 0x80135D3C, Size: 52 bytes (13 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern s16 D_800D9248;

void func_80135D44(void)
{
    s32 temp = D_8012E66C;

    D_800D9248 = (temp * 2) + 13;
    func_8004DE1C();
}
