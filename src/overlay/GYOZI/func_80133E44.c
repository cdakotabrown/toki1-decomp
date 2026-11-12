/* @SymbolStart: 0x80133E3C */
/* @SymbolSize: 44 */
/* func_80133E44 -- GYOZI overlay, Address: 0x80133E3C, Size: 44 bytes (11 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s8 D_800F5ACD;

void func_80133E44(void)
{
    u8 temp = D_800F62CF;

    D_800F5ACD = temp;
    func_8004DE1C();
}
