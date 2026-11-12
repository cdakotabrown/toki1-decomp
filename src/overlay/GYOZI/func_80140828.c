/* func_80140828 -- GYOZI overlay, Address: 0x80140828, Size: 48 bytes (12 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8004DE1C(void);

void func_80140828(void)
{
    func_80051DD8(77, 0x801B0000, 0xB0C5);
    func_8004DE1C();
}
