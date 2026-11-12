/* func_80135B18 -- GYOZI overlay, Address: 0x80135B18, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_801372B0(void);
extern void func_8004DE1C(void);

void func_80135B18(void)
{
    func_80051DD8(19, 0x801B0000, 0xA3C8);
    func_801372B0();
    func_8004DE1C();
}
