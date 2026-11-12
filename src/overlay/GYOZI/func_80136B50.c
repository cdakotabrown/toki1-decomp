/* func_80136B50 -- GYOZI overlay, Address: 0x80136B50, Size: 52 bytes (13 insns) */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_80090960(s32 a0, s32 a1);
extern void func_8004DE1C(void);

void func_80136B50(void)
{
    func_8008A0D4(16365);
    func_80090960(6, 0);
    func_8004DE1C();
}
