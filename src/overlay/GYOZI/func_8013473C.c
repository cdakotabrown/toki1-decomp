/* @SymbolStart: 0x80134734 */
/* @SymbolSize: 56 */
/* func_8013473C -- GYOZI overlay, Address: 0x80134734, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F62CF;

void func_8013473C(void)
{
    u32 temp = D_800F62CF;

    if (temp == 2) {
        func_8004DE1C();
    }
    func_8004DE1C();
}
