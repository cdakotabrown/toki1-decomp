/* func_8013A8E0 -- GYOZI overlay, Address: 0x8013A8E0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8013C3B0(void);
extern void func_8004DE1C(void);

void func_8013A8E0(void)
{
    func_80051DD8(113, 0x801AE000, 0x8453);
    func_8013C3B0();
    func_8004DE1C();
}
