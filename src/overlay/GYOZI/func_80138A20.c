/* func_80138A20 -- GYOZI overlay, Address: 0x80138A20, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_80086AB0(s32);
extern void func_8004DE1C(void);

void func_80138A20(void)
{
    func_80086AB0(0x500);
    func_8004DE1C();
}
