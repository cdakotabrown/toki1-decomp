/* func_801325E0 -- GYOZI overlay, Address: 0x801325E0, Size: 60 bytes (15 insns) */
#include "types.h"

extern s32 func_80050AB8(void);
extern void func_80086AB0(s32);
extern void func_8004DE1C(void);

void func_801325E0(void)
{
    if (func_80050AB8() != 1) {
        return;
    }
    func_80086AB0(512);
    func_8004DE1C();
}
