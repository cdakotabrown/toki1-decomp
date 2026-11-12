/* @SymbolStart: 0x8013383C */
/* @SymbolSize: 84 */
/* func_80133844 -- GYOZI overlay, Address: 0x8013383C, Size: 84 bytes (21 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern s16 D_80145EB4;

void func_80133844(void)
{
    s32 temp = D_8012E66C;

    if (temp != 0) {
        D_80145EB4 = D_80145EB4 + 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
    func_8004DE1C();
}
