/* @SymbolSize: 48 */
/* func_8013527C -- GYOZI overlay, Address: 0x8013527C, Size: 48 bytes (12 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8004DE1C(void);

void func_8013527C(void)
{
    func_80051DD8(55, 0x801C0000, 0xB081);
    func_8004DE1C();
}
