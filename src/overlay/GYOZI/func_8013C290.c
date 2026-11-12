/* func_8013C290 -- GYOZI overlay, Address: 0x8013C290, Size: 60 bytes (15 insns) */
#include "types.h"

extern s32 func_80050AB8(void);
extern void func_80086AB0(s32 a0);
extern void func_8004DE1C(void);

void func_8013C290(void)
{
    if (func_80050AB8() == 1) {
        func_80086AB0(514);
        func_8004DE1C();
    }
}
