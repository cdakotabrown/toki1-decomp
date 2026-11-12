/* func_80136998 -- GYOZI overlay, Address: 0x80136998, Size: 60 bytes (15 insns) */
#include "types.h"

extern s32 func_80050AB8(void);
extern void func_80086AB0(s32 a0);
extern void func_8004DE1C(void);

void func_80136998(void)
{
    if (func_80050AB8() == 1) {
        func_80086AB0(513);
        func_8004DE1C();
    }
}
