/* func_80138E4C -- GYOZI overlay, Address: 0x80138E4C, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

void func_80138E4C(void)
{
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}
