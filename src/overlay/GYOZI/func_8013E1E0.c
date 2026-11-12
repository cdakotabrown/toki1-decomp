/* func_8013E1E0 -- GYOZI overlay, Address: 0x8013E1E0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8013EB10(void);
extern void func_8004DE1C(void);

void func_8013E1E0(void)
{
    func_80051DD8(14, 0x80197000, 0xA474);
    func_8013EB10();
    func_8004DE1C();
}
