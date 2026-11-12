/* func_801369D4 -- GYOZI overlay, Address: 0x801369D4, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80138510(void);
extern void func_8004DE1C(void);

void func_801369D4(void)
{
    func_80051DD8(7, 0x80197000, 0xA464);
    func_80138510();
    func_8004DE1C();
}
