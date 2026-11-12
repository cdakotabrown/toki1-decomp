/* @SymbolStart: 0x801341A8 */
/* @SymbolSize: 44 */
/* func_801341B0 -- GYOZI overlay, Address: 0x801341A8, Size: 44 bytes (11 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern s32 D_8012E67C;

void func_801341B0(void)
{
    s32 temp = D_8012E66C;

    D_8012E67C = temp;
    func_8004DE1C();
}
