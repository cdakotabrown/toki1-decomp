/* @SymbolStart: 0x8013E894 */
/* @SymbolSize: 48 */
/* func_8013E89C -- GYOZI overlay, Address: 0x8013E894, Size: 48 bytes (12 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_8012B8C3;

void func_8013E89C(void)
{
    u32 temp = D_8012B8C3;

    D_8012B8C3 = temp | 0x80;
    func_8004DE1C();
}
