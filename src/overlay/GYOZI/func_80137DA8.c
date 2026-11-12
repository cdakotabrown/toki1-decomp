/* func_80137DA8 -- GYOZI overlay, Address: 0x80137DA8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80137524(void);
extern void func_8004DE1C(void);

void func_80137DA8(void)
{
    func_80051DD8(3, 0x801B0000, 0xA3FB);
    func_80137524();
    func_8004DE1C();
}
