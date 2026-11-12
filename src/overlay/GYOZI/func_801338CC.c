/* @PostProc: pad_trailing_nop --count 3 */
/* func_801338CC -- GYOZI overlay, Address: 0x801338CC, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_80145EB4;

void func_801338CC(void)
{
    D_80145EB4 = 15;
    func_8004DE1C();
}
