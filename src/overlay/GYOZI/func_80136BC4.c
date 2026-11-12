/* @SymbolStart: 0x80136BAC */
/* @SymbolSize: 68 */
/* func_80136BC4 -- GYOZI overlay, Address: 0x80136BAC (prologue 0x80136BC4), */
/* Size: 68 bytes (17 insns).  D_80147198 += D_800F53DE * 3 - 288, then func_8004DE1C(). */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F53DE;
extern s16 D_80147198;

void func_80136BC4(void)
{
    D_80147198 = D_80147198 + D_800F53DE * 3 - 288;
    func_8004DE1C();
}
