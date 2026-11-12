/* func_80139D28 -- GYOZI overlay, Address: 0x80139D28, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

void func_80139D28(void)
{
    func_8008A0D4(0x4011);
    func_8004DE1C();
}
