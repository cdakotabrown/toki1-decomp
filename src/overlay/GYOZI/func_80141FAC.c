/* func_80141FAC -- GYOZI overlay, Address: 0x80141FAC, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80143AD0(void);
extern void func_8004DE1C(void);

void func_80141FAC(void)
{
    func_80051DD8(85, 0x801B0000, 32120);
    func_80143AD0();
    func_8004DE1C();
}
