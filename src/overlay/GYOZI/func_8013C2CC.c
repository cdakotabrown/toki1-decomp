/* func_8013C2CC -- GYOZI overlay, Address: 0x8013C2CC, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8013DE30(void);
extern void func_8004DE1C(void);

void func_8013C2CC(void)
{
    func_80051DD8(9, 0x80197000, 0xA46B);
    func_8013DE30();
    func_8004DE1C();
}
