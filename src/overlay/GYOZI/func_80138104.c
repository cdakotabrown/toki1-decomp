/* func_80138104 -- GYOZI overlay, Address: 0x80138104, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_8004DEAC(s32);
extern void func_8004DEE4(s32);

extern u8 D_800F5AB1;
extern u8 D_800F5AB2;

void func_80138104(void)
{
    func_8004DEAC(D_800F5AB1);
    func_8004DEE4(D_800F5AB2);
}
