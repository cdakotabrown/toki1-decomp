/* func_80139D50 -- GYOZI overlay, Address: 0x80139D50, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

void func_80139D50(void)
{
    func_8008A0D4(0x4080);
    func_8004DE1C();
}
