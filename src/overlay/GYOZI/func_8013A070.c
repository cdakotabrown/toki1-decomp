/* func_8013A070 -- GYOZI overlay, Address: 0x8013A070, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8013BEA0(void);
extern void func_8004DE1C(void);

void func_8013A070(void)
{
    func_80051DD8(113, 0x801B0000, 0x82A1);
    func_8013BEA0();
    func_8004DE1C();
}
