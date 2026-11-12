/* @SymbolStart: 0x801340DC */
/* @SymbolSize: 72 */
/* func_801340E4 -- GYOZI overlay, Address: 0x801340DC, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8012E66C;

void func_801340E4(void)
{
    s32 temp = D_8012E66C;

    if (temp == 2) goto label0;
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();

label0:
    func_8004DE1C();
}
