/* func_801397B4 -- GYOZI overlay, Address: 0x801397B4, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

void func_801397B4(void)
{
    func_8008A0D4(0x3FFF);
    func_8004DE1C();
}
