/* func_80140DF0 -- GYOZI overlay, Address: 0x80140DF0, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80142D00(void);
extern void func_8004DE1C(void);

void func_80140DF0(void)
{
    func_80051DD8(13, 0x801B0000, 0xAFE0);
    func_80142D00();
    func_8004DE1C();
}
