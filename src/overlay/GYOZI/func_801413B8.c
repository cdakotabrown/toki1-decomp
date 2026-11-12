/* func_801413B8 -- GYOZI overlay, Address: 0x801413B8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_80086AB0(s32);
extern void func_8004DE1C(void);

void func_801413B8(void)
{
    func_80086AB0(0x500);
    func_8004DE1C();
}
