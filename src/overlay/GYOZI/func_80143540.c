/* func_80143540 -- GYOZI overlay, Address: 0x80143540, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80143C70(void);
extern void func_8004DE1C(void);

void func_80143540(void)
{
    func_80051DD8(69, 0x801B0000, 0x88B4);
    func_80143C70();
    func_8004DE1C();
}
