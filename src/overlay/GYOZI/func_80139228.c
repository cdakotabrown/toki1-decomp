/* func_80139228 -- GYOZI overlay, Address: 0x80139228, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_80086AB0(s32);
extern void func_8004DE1C(void);

void func_80139228(void)
{
    func_80086AB0(0x500);
    func_8004DE1C();
}
