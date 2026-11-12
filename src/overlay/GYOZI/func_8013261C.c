/* func_8013261C -- GYOZI overlay, Address: 0x8013261C, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80134000(void);
extern void func_8004DE1C(void);

void func_8013261C(void)
{
    func_80051DD8(51, 0x80197000, 0xA400);
    func_80134000();
    func_8004DE1C();
}
