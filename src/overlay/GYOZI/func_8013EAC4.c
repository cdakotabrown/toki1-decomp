/* func_8013EAC4 -- GYOZI overlay, Address: 0x8013EAC4, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80140740(void);
extern void func_8004DE1C(void);

void func_8013EAC4(void)
{
    func_80051DD8(13, 0x801A0000, 0xB0B8);
    func_80140740();
    func_8004DE1C();
}
