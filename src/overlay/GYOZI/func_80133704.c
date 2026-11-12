/* @SymbolStart: 0x801336FC */
/* @SymbolSize: 52 */
/* func_80133704 -- GYOZI overlay, Address: 0x801336FC, Size: 52 bytes (13 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8012E66C;

void func_80133704(void)
{
    s32 temp = D_8012E66C;

    if (temp == 0) {
        func_8004DE1C();
    }
    func_8004DE1C();
}
