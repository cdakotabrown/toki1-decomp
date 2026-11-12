/* func_80140858 -- GYOZI overlay, Address: 0x80140858, Size: 52 bytes (13 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8004DE1C(void);

void func_80140858(void)
{
    func_80051DD8(14, 0x801D7000, 0xB112);
    func_8004DE1C();
}
