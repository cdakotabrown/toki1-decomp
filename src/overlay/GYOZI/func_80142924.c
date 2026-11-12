/* @SymbolStart: 0x8014291C */
/* @SymbolSize: 48 */
/* func_80142924 -- GYOZI overlay, Address: 0x8014291C, Size: 48 bytes (12 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_8012B94B;

void func_80142924(void)
{
    u32 temp = D_8012B94B;

    D_8012B94B = temp & 0x7F;
    func_8004DE1C();
}
